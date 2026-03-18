
#include "AppSetup.h"
#include "CrapsBonusTracker.h"
#include "CbtPresenter.h"
#include "NcursesView.h"

#include <spdlog/spdlog.h>
#include <memory>

int main(int argc, char *argv[]) {

    // Set up Logger
    AppSetup::CreateLogger();

    SPDLOG_INFO("Starting CrapsBonusTracker...");

    auto model = std::make_shared<CrapsBonusTracker>();
    NcursesView view;
    CbtPresenter presenter(model, view);
    presenter.Run();

    return 0;
}

