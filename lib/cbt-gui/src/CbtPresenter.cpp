#include "CbtPresenter.h"

#include <spdlog/spdlog.h>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include "CrapsBonusTracker.h"

/**
 * @brief Concrete Presenter for the CrapsBonusTracker MVP architecture.
 *
 * @param model Shared ownership of the bonus tracker object.
 * @param view  Reference to the active View implementation.
 */
CbtPresenter::CbtPresenter(std::shared_ptr<CrapsBonusTracker> Model, IView& View) :
    m_Model(std::move(Model)),
    m_View(View),
    m_Running(true)
{
    if (!m_Model)
    {
        SPDLOG_CRITICAL("CBT Model must not be NULL");
        throw std::invalid_argument("CBT Model must not be NULL");
    }
}

/** 
 * @copydoc IPresenter::Run
 *
 */
void CbtPresenter::Run()
{
    m_View.Init();
    SPDLOG_TRACE("CbtPresenter::Run() started");

    m_View.ShowBonusNumbers(m_Model->GetRollMap());
    m_View.ShowLastRoll(0);
    m_View.ShowWins(m_Model->GetSmallWin(), m_Model->GetAllWin(), m_Model->GetTallWin());

    while (m_Running)
    {
        int roll = m_View.PromptDiceRoll();
        if (roll == 0)
        {
            m_Running = false;
            break; 
        }          

        m_Model->MarkNumber(static_cast<uint8_t>(roll));
        m_View.ShowLastRoll(roll);
        m_View.ShowBonusNumbers(m_Model->GetRollMap());
        m_View.ShowWins(m_Model->GetSmallWin(), m_Model->GetAllWin(), m_Model->GetTallWin());
        
    }

    m_View.Shutdown();
    SPDLOG_TRACE("CbtPresenter::Run() exited");
}
