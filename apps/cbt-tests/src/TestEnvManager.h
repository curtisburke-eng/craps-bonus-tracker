#pragma once

#include "CrapsBonusTracker.h"
#include <memory>

class TestEnvManager
{
public:
    static std::shared_ptr<CrapsBonusTracker> GetCbtInstance();
    static void SetCbtInstance(std::shared_ptr<CrapsBonusTracker> Cbt);

private:
    static std::shared_ptr<CrapsBonusTracker> m_Cbt;
};
