#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <system_error>

#include "CrapsBonusTracker.h"

class CbtTestBase : public ::testing::Test {

protected:
    void SetUp() override;

    std::shared_ptr<CrapsBonusTracker> m_Cbt;
};

