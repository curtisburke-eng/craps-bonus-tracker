
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
    })
{

}


std::error_code CrapsBonusTracker::MarkNumber(uint8_t CurrentRoll)
{
    if(CurrentRoll < 2 || CurrentRoll > 12)
    {
        SPDLOG_ERROR("Roll must be between 2 and 12");
        return eInvalidArgument;
    }
    
    if(CurrentRoll == 7)
    {
        Reset();
    }
    else if (m_rollMap.at(CurrentRoll) == false) {
        m_rollMap.at(CurrentRoll) = true;
    }

    return eOk;
}


void CrapsBonusTracker::Reset()
{
    for (auto& pair : m_rollMap) {
        pair.second = false;
    }
}

