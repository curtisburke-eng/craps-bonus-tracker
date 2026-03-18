
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


TEST_F(CrapsBonusTrackerTest, UpdateWins_small)
{
    // Get the roll map before marking
    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
    // Ensure initial win is false
    EXPECT_FALSE(m_Cbt->GetSmallWin());

    // Mark all the numbers 2-6
    for (int i = 2; i <= 6; i++) {
        
        std::error_code retVal = m_Cbt->MarkNumber(i);
        ASSERT_EQ(retVal, CbtErrorCodes::eOk);
    }
    
    // Get the roll map after marking
    rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 6; i++) {
        EXPECT_TRUE(rollMap.at(i));
    }

    // Ensure win is true
    EXPECT_TRUE(m_Cbt->GetSmallWin());
    
}

TEST_F(CrapsBonusTrackerTest, UpdateWins_tall)
{
    // Get the roll map before marking
    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
    // Ensure initial win is false
    EXPECT_FALSE(m_Cbt->GetTallWin());

    // Mark all the numbers 8-12
    for (int i = 8; i <= 12; i++) {
        
        std::error_code retVal = m_Cbt->MarkNumber(i);
        ASSERT_EQ(retVal, CbtErrorCodes::eOk);
    }
    
    // Get the roll map after marking
    rollMap = m_Cbt->GetRollMap();
    for (int i = 8; i <= 12; i++) {
        EXPECT_TRUE(rollMap.at(i));
    }

    // Ensure win is true
    EXPECT_TRUE(m_Cbt->GetTallWin());
    
}

TEST_F(CrapsBonusTrackerTest, UpdateWins_All)
{
    // Get the roll map before marking
    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
    // Ensure initial win is false
    EXPECT_FALSE(m_Cbt->GetSmallWin());
    EXPECT_FALSE(m_Cbt->GetTallWin());
    EXPECT_FALSE(m_Cbt->GetAllWin());

    // Mark all the numbers (skipping 7)
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

    // Ensure win is true
    EXPECT_TRUE(m_Cbt->GetSmallWin());
    EXPECT_TRUE(m_Cbt->GetTallWin());
    EXPECT_TRUE(m_Cbt->GetAllWin());
    
}

TEST_F(CrapsBonusTrackerTest, SevenResetsWins)
{
    // Get the roll map before marking
    std::map<uint8_t, bool> rollMap = m_Cbt->GetRollMap();
    for (int i = 2; i <= 12; i++) {
        if (i == 7) {
            continue;
        }

        EXPECT_FALSE(rollMap.at(i));
    }
    // Ensure initial win is false
    EXPECT_FALSE(m_Cbt->GetSmallWin());
    EXPECT_FALSE(m_Cbt->GetTallWin());
    EXPECT_FALSE(m_Cbt->GetAllWin());

    // Mark all the numbers (skipping 7)
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

    // Ensure win is true
    EXPECT_TRUE(m_Cbt->GetSmallWin());
    EXPECT_TRUE(m_Cbt->GetTallWin());
    EXPECT_TRUE(m_Cbt->GetAllWin());

    // Roll a 7 (to reset the map and wins)
    std::error_code retVal = m_Cbt->MarkNumber(7);
    ASSERT_EQ(retVal, CbtErrorCodes::eOk);
    
    // Ensure initial win is false
    EXPECT_FALSE(m_Cbt->GetSmallWin());
    EXPECT_FALSE(m_Cbt->GetTallWin());
    EXPECT_FALSE(m_Cbt->GetAllWin());
}