#pragma once

/**
 * @brief Pure interface for the Presenter layer of the MVP architecture.
 *
 * The View holds a reference to IPresenter and calls it in response to
 * user input events. The concrete implementation (CbtPresenter) translates
 * those events into Model calls and drives View transitions.
 */
class IPresenter
{
public:
    virtual ~IPresenter() = default;

    /**
     * @brief Start the main application loop.
     *
     * Initializes the View and runs the FSM until the user quits.
     * Blocks until the application exits.
     */
    virtual void Run() = 0;
};
