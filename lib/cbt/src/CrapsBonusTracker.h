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
    bool GetSmallWin() { return m_smallWin;};
    bool GetTallWin() { return m_tallWin;};
    bool GetAllWin() { return m_allWin;};

    std::error_code MarkNumber(uint8_t CurrentRoll);
    
    void Reset();

private:
    void UpdateWins();

    std::map<uint8_t, bool> m_rollMap;
    bool m_smallWin;
    bool m_tallWin;
    bool m_allWin;
    
};
