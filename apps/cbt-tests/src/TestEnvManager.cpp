
#include "TestEnvManager.h"
#include "CrapsBonusTracker.h"

/**
 * @brief Static member initialization.
 */
std::shared_ptr<CrapsBonusTracker> TestEnvManager::m_Cbt = nullptr;

/**
 * @brief Retrieves the CBT instance.
 * @return A shared pointer to the top level CrapsBonusTracker object.
 */
std::shared_ptr<CrapsBonusTracker> TestEnvManager::GetCbtInstance()
{
    return m_Cbt;
}

/**
 * @brief Sets the CBT instance.
 * @param Cbt A shared pointer to the CrapsBonusTracker object to set.
 */
void TestEnvManager::SetCbtInstance(std::shared_ptr<CrapsBonusTracker> Cbt)
{
    m_Cbt = Cbt;
}