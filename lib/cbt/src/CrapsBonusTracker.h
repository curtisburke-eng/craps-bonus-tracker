#pragma once

#include <map>
#include <memory>
#include <system_error>


class CrapsBonusTracker
{
public:
    CrapsBonusTracker();
    ~CrapsBonusTracker() = default;

    std::map<uint8_t, bool> GetRollMap() { return m_rollMap; };
    std::error_code MarkNumber(uint8_t CurrentRoll);
    
    void Reset();

private:
    std::map<uint8_t, bool> m_rollMap;
    
};
