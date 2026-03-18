
#include "CrapsBonusTrackerTest.h"

#include <gtest/gtest.h>
#include <system_error>
#include "CrapsBonusTracker.h"
#include "utils/CbtErrorCodes.h"

// --------------------- CONSTRUCTION TESTS ---------------------

TEST_F(CrapsBonusTrackerTest, Constructor_CreatesValidInstance)
{
    ASSERT_NO_THROW(CrapsBonusTracker());
}

TEST_F(CrapsBonusTrackerTest, MarkNumber)
{
    uint8_t testNumber = 2;

    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    EXPECT_FALSE(rollMap.at(testNumber));
    
    std::error_code retVal = m_Cbt->MarkNumber(testNumber);
    ASSERT_EQ(retVal, CbtErrorCodes::eOk);

    rollMap = m_Cbt->GetRollMap();
    EXPECT_TRUE(rollMap.at(testNumber));

}

TEST_F(CrapsBonusTrackerTest, MarkAllNumbers)
{

    // Get the roll map before marking
    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
    
    // Mark all the numbers 2-12 (skipping 7)
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }
        
        std::error_code retVal = m_Cbt->MarkNumber(i);
        ASSERT_EQ(retVal, CbtErrorCodes::eOk);
    }
    
    // Get the roll map after marking
    rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_TRUE(rollMap.at(i));
    }
    
}

TEST_F(CrapsBonusTrackerTest, MarkAllNumbers_ThenAutoReset)
{
    // Get the roll map before marking
    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
    
    // Mark all the numbers 2-12 (skipping 7)
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }
        
        std::error_code retVal = m_Cbt->MarkNumber(i);
        ASSERT_EQ(retVal, CbtErrorCodes::eOk);
    }
    
    // Get the roll map after marking
    rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_TRUE(rollMap.at(i));
    }

    // Mark the 7 (Resets Map)
    std::error_code retVal = m_Cbt->MarkNumber(7);
    ASSERT_EQ(retVal, CbtErrorCodes::eOk);

    // Get the roll map after resetting
    rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
}

TEST_F(CrapsBonusTrackerTest, MarkNumber_outOfBounds_lower)
{
    std::error_code retVal = m_Cbt->MarkNumber(1);
    ASSERT_EQ(retVal, CbtErrorCodes::eInvalidArgument);
}

TEST_F(CrapsBonusTrackerTest, MarkNumber_outOfBounds_upper)
{
    std::error_code retVal = m_Cbt->MarkNumber(13);
    ASSERT_EQ(retVal, CbtErrorCodes::eInvalidArgument);
}
