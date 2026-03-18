
#include "CrapsBonusTracker.h"

#include <spdlog/spdlog.h>
#include <system_error>

#include "utils/CbtErrorCodes.h"

using enum CbtErrorCodes;


/**
 * @brief Constructor for CrapsBonusTracker. Initializes members
 */
CrapsBonusTracker::CrapsBonusTracker() :
    m_rollMap({
        {2, false}, {3, false}, {4, false}, {5, false}, {6, false},
        {8, false}, {9, false}, {10, false}, {11, false}, {12, false}
    }),
    m_smallWin(false),
    m_tallWin(false),
    m_allWin(false)
{

}


std::error_code CrapsBonusTracker::MarkNumber(uint8_t CurrentRoll)
{
    if(CurrentRoll < 2 || CurrentRoll > 12)
    {
        SPDLOG_ERROR("Roll must be between 2 and 12");
        return eInvalidArgument;
    }
    
    // Reset on 7; otherwise set the rollMap value to true for the current roll
    if(CurrentRoll == 7)
    {
        Reset();
    }
    else if (m_rollMap.at(CurrentRoll) == false) {
        m_rollMap.at(CurrentRoll) = true;
        UpdateWins();
    }

    return eOk;
}


void CrapsBonusTracker::Reset()
{
    // Reset the values for all keys
    for (auto& pair : m_rollMap) {
        pair.second = false;
    }
    // Reset any win booleans
    m_smallWin = false;
    m_tallWin  = false;
    m_allWin   = false;
}

void CrapsBonusTracker::UpdateWins()
{
    // Boolean math; all must be true to make the win true.
    m_smallWin = m_rollMap.at(2) && m_rollMap.at(3) && m_rollMap.at(4)  && m_rollMap.at(5)  && m_rollMap.at(6);
    m_tallWin  = m_rollMap.at(8) && m_rollMap.at(9) && m_rollMap.at(10) && m_rollMap.at(11) && m_rollMap.at(12);
    m_allWin   = m_smallWin && m_tallWin;
}