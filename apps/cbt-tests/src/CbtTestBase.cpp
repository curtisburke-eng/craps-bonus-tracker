
#include "CbtTestBase.h"

#include <gtest/gtest.h>
#include <string>
#include <memory>
#include <system_error>

#include "TestEnvManager.h"

void CbtTestBase::SetUp() {
    m_Cbt = TestEnvManager::GetCbtInstance();

}
