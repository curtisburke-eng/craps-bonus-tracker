
#include "CrapsBonusTrackerTest.h"

#include <gtest/gtest.h>
#include <system_error>
#include "CrapsBonusTracker.h"

// --------------------- CONSTRUCTION TESTS ---------------------

TEST_F(CrapsBonusTrackerTest, Constructor_CreatesValidInstance)
{
    ASSERT_NO_THROW(CrapsBonusTracker());
}
