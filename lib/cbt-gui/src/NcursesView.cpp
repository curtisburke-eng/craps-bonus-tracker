#include "NcursesView.h"

#include <ncurses.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <stdexcept>
#include <string>


//  Color-pair indices
static constexpr int COLOR_NORMAL           = 1;
static constexpr int COLOR_ERROR            = 2;
static constexpr int COLOR_HIGHLIGHT        = 3;
static constexpr int COLOR_HEADER           = 4;
static constexpr int COLOR_BONUS_NUM_HIT    = 5;
static constexpr int COLOR_BONUS_NUM_UNHIT  = 6;
static constexpr int COLOR_ROLL_NUM         = 7;


//  ASCII Art for Big-digit font (5 rows × 5 chars)
//  Each number is represented by 5 strings, each exactly 5 characters wide.
static constexpr int ASCII_DIGIT_ROWS = 5;
static constexpr int ASCII_DIGIT_COLS = 5;   // chars per digit (inter-digit gap baked in)
// clang-format off
static const char* const ASCII_ART_DIGITS[10][5] =
{
    {" ___ ", "|   |", "|   |", "|   |", "|___|" },  // 0
    {"     ", "  |  ", "  |  ", "  |  ", "  |  " },  // 1
    {" ___ ", "    |", " ___ ", "|    ", "|___ " },  // 2
    {" ___ ", "    |", " ___ ", "    |", " ___|"},   // 3
    {"     ", "|   |", "|___|", "    |", "    |" },  // 4
    {" ___ ", "|    ", "|___ ", "    |", " ___|"},   // 5
    {" ___ ", "|    ", "|___ ", "|   |", "|___|" },  // 6
    {" ___ ", "    |", "    |", "    |", "    |" },  // 7
    {" ___ ", "|   |", "|___|", "|   |", "|___|" },  // 8
    {" ___ ", "|   |", "|___|", "    |", " ___|"},   // 9
};
// clang-format on


NcursesView::NcursesView() = default;

NcursesView::~NcursesView()
{
    if (m_Initialized)
        Shutdown();
}

/**
 * @brief Initialise ncurses and create the three panels.
 *
 * Layout:
 *
 *   ┌──────────────────────────────────────────────────────────────────┐
 *   │  Bonus Panel  (top 1/3 rows, full width)                         │
 *   ├────────────────────────────────────────┬─────────────────────────┤
 *   │  Menu Panel                            │  Roll Panel             │
 *   │  (bottom 2/3 rows, left 2/3 width)     │  (right 1/3 width)      │
 *   └────────────────────────────────────────┴─────────────────────────┘
 */
void NcursesView::Init()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();
        init_pair(COLOR_NORMAL,             COLOR_WHITE,  COLOR_BLACK);
        init_pair(COLOR_ERROR,              COLOR_RED,    COLOR_BLACK);
        init_pair(COLOR_HIGHLIGHT,          COLOR_BLACK,  COLOR_CYAN);
        init_pair(COLOR_HEADER,             COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_BONUS_NUM_HIT,      COLOR_GREEN,  COLOR_BLACK);
        init_pair(COLOR_BONUS_NUM_UNHIT,    COLOR_WHITE,  COLOR_BLACK);
        init_pair(COLOR_ROLL_NUM,           COLOR_CYAN,   COLOR_BLACK);
    }

    getmaxyx(stdscr, m_TermRows, m_TermCols);

    const int bonusRows  = m_TermRows / 3;
    const int bottomRows = m_TermRows - bonusRows;
    const int menuCols   = (m_TermCols * 2) / 5;
    const int rollCols   = m_TermCols / 5;
    const int winsCols   = m_TermCols - menuCols - rollCols;

    m_BonusWin = newwin(bonusRows,  m_TermCols, 0,         0);
    m_MenuWin  = newwin(bottomRows, menuCols,   bonusRows, 0);
    m_RollWin  = newwin(bottomRows, rollCols,   bonusRows, menuCols);
    m_WinsWin  = newwin(bottomRows, winsCols,   bonusRows, menuCols + rollCols);

    if (!m_BonusWin || !m_MenuWin || !m_RollWin || !m_WinsWin)
        throw std::runtime_error("NcursesView: failed to create panel windows");

    keypad(m_MenuWin, TRUE);
    scrollok(m_MenuWin, FALSE);
    scrollok(m_RollWin, FALSE);
    scrollok(m_WinsWin, FALSE);

    DrawBorder(m_BonusWin, " CRAPS BONUS GAME ");
    wrefresh(m_BonusWin);

    DrawBorder(m_MenuWin, "");
    wrefresh(m_MenuWin);

    DrawBorder(m_RollWin, " LAST ROLL ");
    wrefresh(m_RollWin);

    DrawBorder(m_WinsWin, " PAYOUTS ");
    wrefresh(m_WinsWin);

    m_Initialized = true;
    SPDLOG_DEBUG("NcursesView initialised ({}x{})", m_TermCols, m_TermRows);
}

void NcursesView::Shutdown()
{
    if (m_BonusWin) { delwin(m_BonusWin); m_BonusWin = nullptr; }
    if (m_MenuWin)  { delwin(m_MenuWin);  m_MenuWin  = nullptr; }
    if (m_RollWin)  { delwin(m_RollWin);  m_RollWin  = nullptr; }
    if (m_WinsWin)  { delwin(m_WinsWin);  m_WinsWin  = nullptr; }
    endwin();
    m_Initialized = false;
    SPDLOG_DEBUG("NcursesView shut down");
}

/**
 * @brief Redraw the bonus panel, marking which numbers have been hit.
 *
 * Displays:  2  3  4  5  6  |  8  9  10  11  12
 * Hit numbers shown bold+green; unhit numbers shown dim.
 */
void NcursesView::ShowBonusNumbers(const std::map<uint8_t, bool>& rollMap)
{
    DrawBonusPanel(rollMap);
}

/**
 * @brief Update the roll panel with the given total in large font.
 * @param roll  Roll total (2-12).  Pass 0 to exit.
 */
void NcursesView::ShowLastRoll(int roll)
{
    m_LastRoll = roll;
    DrawRollPanel(roll);
}

void NcursesView::ShowWins(bool smallWin, bool allWin, bool tallWin)
{
    DrawWinsPanel(smallWin, allWin, tallWin);
}


int NcursesView::PromptDiceRoll()
{
    while (true)
    {
        werase(m_MenuWin);
        DrawBorder(m_MenuWin, " INPUT ");
        wrefresh(m_MenuWin);

        std::string input = ReadLine("Enter Roll total (2-12, 0 to exit): ", 4);
        try
        {
            int roll = std::stoi(input);
            if (roll == 0)
            {
                DrawRollPanel(0);
                return 0;
            }
            if (roll >= 2 && roll <= 12)
            {
                DrawRollPanel(roll);
                return roll;
            }
        }
        catch (const std::exception&) {}

        PrintStatus("Invalid — enter 2-12, or 0 to exit.", true);
        WaitForKey();
    }
}


/**
 * @brief Render the bonus panel.
 *
 * Numbers laid out as:  2  3  4  5  6  |  8  9  10  11  12
 * Hit numbers are green+bold; unhit numbers are dim.
 */
void NcursesView::DrawBonusPanel(const std::map<uint8_t, bool>& rollMap)
{
    werase(m_BonusWin);
    DrawBorder(m_BonusWin, " CRAPS BONUS ");

    const int panelCols = getmaxx(m_BonusWin);
    const int panelRows = getmaxy(m_BonusWin);
    const int centerRow = panelRows / 2;

    static const uint8_t kNumbers[] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12};

    // 5 slots × 4 chars + separator " | " (3 chars) + 5 slots × 4 chars = 43 chars total.
    static constexpr int kSlotWidth = 4;
    static constexpr int kSepWidth  = 3;
    const int totalWidth = 5 * kSlotWidth + kSepWidth + 5 * kSlotWidth;
    int col = std::max(2, (panelCols - totalWidth) / 2);

    for (int i = 0; i < 10; ++i)
    {
        uint8_t num = kNumbers[i];

        if (i == 5)
        {
            wattron(m_BonusWin, COLOR_PAIR(COLOR_HEADER) | A_BOLD);
            mvwprintw(m_BonusWin, centerRow, col, " | ");
            wattroff(m_BonusWin, COLOR_PAIR(COLOR_HEADER) | A_BOLD);
            col += kSepWidth;
        }

        bool hit = false;
        auto it = rollMap.find(num);
        if (it != rollMap.end())
            hit = it->second;

        if (hit)
        {
            wattron(m_BonusWin, COLOR_PAIR(COLOR_BONUS_NUM_HIT) | A_BOLD);
            mvwprintw(m_BonusWin, centerRow, col, "%2d  ", num);
            wattroff(m_BonusWin, COLOR_PAIR(COLOR_BONUS_NUM_HIT) | A_BOLD);
        }
        else
        {
            wattron(m_BonusWin, COLOR_PAIR(COLOR_BONUS_NUM_UNHIT) | A_DIM);
            mvwprintw(m_BonusWin, centerRow, col, "%2d  ", num);
            wattroff(m_BonusWin, COLOR_PAIR(COLOR_BONUS_NUM_UNHIT) | A_DIM);
        }

        col += kSlotWidth;
    }

    wrefresh(m_BonusWin);
}

/**
 * @brief Render the roll panel with a large-font roll number centred inside.
 * @param roll  Number to display (2-12); 0 draws a blank panel.
 */
void NcursesView::DrawRollPanel(int roll)
{
    werase(m_RollWin);
    DrawBorder(m_RollWin, " LAST ROLL ");

    if (roll > 0)
    {
        const int panelRows = getmaxy(m_RollWin);
        const int panelCols = getmaxx(m_RollWin);

        int digits   = (roll >= 10) ? 2 : 1;
        int numWidth = digits * ASCII_DIGIT_COLS;
        int startCol = std::max(1, (panelCols - numWidth) / 2);
        int startRow = std::max(1, (panelRows - ASCII_DIGIT_ROWS) / 2);

        wattron(m_RollWin, COLOR_PAIR(COLOR_ROLL_NUM) | A_BOLD);
        DrawBigNumber(m_RollWin, startRow, startCol, roll);
        wattroff(m_RollWin, COLOR_PAIR(COLOR_ROLL_NUM) | A_BOLD);
    }

    wrefresh(m_RollWin);
}

/**
 * @brief Render the wins panel showing MAKE 'EM SMALL/ALL/TALL status.
 */
void NcursesView::DrawWinsPanel(bool smallWin, bool allWin, bool tallWin)
{
    werase(m_WinsWin);
    DrawBorder(m_WinsWin, " PAYOUTS ");

    auto drawLine = [&](int row, bool win, const char* label)
    {
        if (win)
        {
            wattron(m_WinsWin, COLOR_PAIR(COLOR_BONUS_NUM_HIT) | A_BOLD);
            mvwprintw(m_WinsWin, row, 2, "WINNER!!! - %s", label);
            wattroff(m_WinsWin, COLOR_PAIR(COLOR_BONUS_NUM_HIT) | A_BOLD);
        }
        else
        {
            wattron(m_WinsWin, COLOR_PAIR(COLOR_BONUS_NUM_UNHIT) | A_DIM);
            mvwprintw(m_WinsWin, row, 2, "%s", label);
            wattroff(m_WinsWin, COLOR_PAIR(COLOR_BONUS_NUM_UNHIT) | A_DIM);
        }
    };

    drawLine(2, smallWin, "MAKE 'EM SMALL");
    drawLine(4, allWin,   "MAKE 'EM ALL");
    drawLine(6, tallWin,  "MAKE 'EM TALL");

    wrefresh(m_WinsWin);
}

/**
 * @brief Draw an integer in large font at (startRow, startCol).
 */
void NcursesView::DrawBigNumber(WINDOW* win, int startRow, int startCol, int num)
{
    if (num >= 10)
        DrawBigDigit(win, startRow, startCol, num / 10);

    int tensWidth = (num >= 10) ? ASCII_DIGIT_COLS : 0;
    DrawBigDigit(win, startRow, startCol + tensWidth, num % 10);
}

/**
 * @brief Render one digit (0-9) of the big-digit font at (row, col).
 */
void NcursesView::DrawBigDigit(WINDOW* win, int row, int col, int digit)
{
    if (digit < 0 || digit > 9)
        return;

    for (int r = 0; r < ASCII_DIGIT_ROWS; ++r)
        mvwprintw(win, row + r, col, "%s", ASCII_ART_DIGITS[digit][r]);
}

/**
 * @brief Draw a titled box around a window.
 */
void NcursesView::DrawBorder(WINDOW* win, const std::string& title)
{
    box(win, 0, 0);
    if (!title.empty())
    {
        int cols = getmaxx(win);
        int x    = std::max(1, (cols - static_cast<int>(title.size())) / 2);
        wattron(win, A_BOLD);
        mvwprintw(win, 0, x, "%s", title.c_str());
        wattroff(win, A_BOLD);
    }
}

/**
 * @brief Print a status hint in the bottom border row of the menu panel.
 */
void NcursesView::PrintStatus(const std::string& msg, bool isError)
{
    int colourPair = isError ? COLOR_ERROR : COLOR_NORMAL;
    wattron(m_MenuWin, COLOR_PAIR(colourPair));
    mvwprintw(m_MenuWin, getmaxy(m_MenuWin) - 1, 2,
              " %-*s ", getmaxx(m_MenuWin) - 5, msg.c_str());
    wattroff(m_MenuWin, COLOR_PAIR(colourPair));
    wrefresh(m_MenuWin);
}

/**
 * @brief Block until the user presses any key.
 */
void NcursesView::WaitForKey()
{
    mvwprintw(m_MenuWin, getmaxy(m_MenuWin) - 1, 2,
              " Press any key to continue... ");
    wrefresh(m_MenuWin);
    curs_set(0);
    wgetch(m_MenuWin);
}

/**
 * @brief Read a line of text from the menu window.
 */
std::string NcursesView::ReadLine(const std::string& prompt, int row)
{
    curs_set(1);
    echo();

    mvwprintw(m_MenuWin, row, 4, "%s", prompt.c_str());
    wrefresh(m_MenuWin);

    char buf[256] = {};
    wgetnstr(m_MenuWin, buf, sizeof(buf) - 1);

    noecho();
    curs_set(0);
    return std::string(buf);
}

