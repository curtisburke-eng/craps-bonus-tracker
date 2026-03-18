#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Pure interface for the View layer of the MVP architecture.
 *
 * The Presenter drives all screen transitions by calling these methods.
 * A concrete implementation (e.g. NcursesView) handles all rendering and
 * raw input. Swapping to a different UI toolkit only requires a new
 * implementation of this interface.
 */
class IView
{
public:
    virtual ~IView() = default;

    // ---- Applicaiton Lifecycle ----

    /** Initialize the UI subsystem (e.g. ncurses initscr()). */
    virtual void Init()     = 0;

    /** Tear down the UI subsystem cleanly. */
    virtual void Shutdown() = 0;

    // ---- Screen Rendering ---- 

    /**
     * @brief Render the bonus-number panel, marking which numbers have been hit.
     * @param rollMap  Map of bonus numbers (2-6, 8-12) to hit state.
     */
    virtual void ShowBonusNumbers(const std::map<uint8_t, bool>& rollMap) = 0;

    /**
     * @brief Display the most-recent roll in large font on the roll panel.
     * @param roll  The roll total to display (2-12, or 0 for blank).
     */
    virtual void ShowLastRoll(int roll) = 0;

    /**
     * @brief Update the bonus pays panel with current win state.
     * @param smallWin  True if all of 2-6 have been hit.
     * @param allWin    True if all 10 bonus numbers have been hit.
     * @param tallWin   True if all of 8-12 have been hit.
     */
    virtual void ShowWins(bool smallWin, bool allWin, bool tallWin) = 0;

    // ---- User Input ---- 

    /**
     * @brief Prompt the user to enter their roll total.
     * @return The roll total (2-12).
     */
    virtual int PromptDiceRoll() = 0;

};
