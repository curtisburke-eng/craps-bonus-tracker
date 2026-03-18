#pragma once

#include "IView.h"

#include <cstdint>
#include <map>
#include <string>

// Forward-declare the ncurses WINDOW type to avoid pulling ncurses.h into
// every translation unit that includes this header.
struct _win_st;
using WINDOW = struct _win_st;


class NcursesView : public IView
{
public:
    NcursesView();
    ~NcursesView()  override;

    //  Lifecycle
    void Init()     override;
    void Shutdown() override;

    //  Screen rendering
    void ShowBonusNumbers(const std::map<uint8_t, bool>& rollMap)  override;
    void ShowLastRoll(int roll)                                    override;
    void ShowWins(bool smallWin, bool allWin, bool tallWin)        override;

    //  User input
    int PromptDiceRoll() override;

private:

    //  Panel drawing helpers
    void DrawBonusPanel(const std::map<uint8_t, bool>& rollMap);
    void DrawRollPanel(int roll);
    void DrawWinsPanel(bool smallWin, bool allWin, bool tallWin);
    void DrawBigNumber(WINDOW* win, int startRow, int startCol, int num);
    void DrawBigDigit(WINDOW* win, int row, int col, int digit);

    //  General helpers
    void        DrawBorder(WINDOW* win, const std::string& title);
    void        PrintStatus(const std::string& msg, bool isError = false);
    void        WaitForKey();
    std::string ReadLine(const std::string& prompt, int row);

    //  Window panels
    WINDOW* m_BonusWin = nullptr;   // Top 1/3, full width  — bonus number grid
    WINDOW* m_MenuWin  = nullptr;   // Bottom section, left 2/5  — menu / input area
    WINDOW* m_RollWin  = nullptr;   // Bottom section, middle 1/5 — last roll (big font)
    WINDOW* m_WinsWin  = nullptr;   // Bottom section, right 2/5  — bonus pays

    bool m_Initialized = false;

    // Terminal dimensions captured at Init()
    int m_TermRows = 0;
    int m_TermCols = 0;

    // Most-recent roll displayed in the roll panel (0 = blank)
    int m_LastRoll = 0;
};
