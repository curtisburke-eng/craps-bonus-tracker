
#include "AppSetup.h"
#include "CrapsBonusTracker.h"

#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {

    // Set up Logger
    AppSetup::CreateLogger();

    SPDLOG_INFO("Starting CrapsBonusTracker...");


    return 0;
}

