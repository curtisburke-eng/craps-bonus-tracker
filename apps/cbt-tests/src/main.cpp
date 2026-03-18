
#include "AppSetup.h"

#include <gtest/gtest.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "TestEnvManager.h"

int main(int argc, char* argv[]) {

    // Set up Logger
    AppSetup::CreateLogger();

    std::shared_ptr<CrapsBonusTracker> cbt = std::make_shared<CrapsBonusTracker>();

    TestEnvManager::SetCbtInstance(cbt);

    SPDLOG_INFO("Starting cbt-tests...");
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    spdlog::shutdown();

    return result;
}
