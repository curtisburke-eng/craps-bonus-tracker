
#include "AppSetup.h"

#include <filesystem>
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/**
 * @brief Creates and configures the logger for the application.
 *
 * Sets up a rotating file logger and a console logger with specified
 * maximum size and number of rotated files.
 */
void AppSetup::CreateLogger()
{
    // Construct log file path
    std::string logFilePath = "./logs/craps-bonus-tracker.log";
    // Create a rotating file logger with 250 MB size max and 10 rotated files
    const int ONE_MB = 1024 * 1024;  // 1,048,576 bytes
    const int MAX_LOG_SIZE = ONE_MB * 250;
    const int MAX_FILES = 10;

    std::vector<spdlog::sink_ptr> sinks;

    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFilePath, MAX_LOG_SIZE, MAX_FILES);
    fileSink->set_level(spdlog::level::trace); // Set sink level - Second filter: if a message passes the logger level filter, each sink then applies its own filter
    sinks.push_back(fileSink);

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::trace); // Set sink level - Second filter: if a message passes the logger level filter, each sink then applies its own filter
    sinks.push_back(consoleSink);

    // Create default logger with sinks
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("CBT", sinks.begin(), sinks.end());
    spdlog::set_default_logger(logger);

    // Set logger pattern to "[timestamp] [log level] [source_file:line_number] message"
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");

    logger->set_level(spdlog::level::info);  // Set global Logger level - First filter: if a message's level is below the logger's level, it's rejected immediately without checking sinks level
    spdlog::flush_on(spdlog::level::err);   // flush_on tells spdlog to write immediately all pending buffered messages at or above this level.
                                            // This ensures critical error messages aren't lost if the application crashes shortly after logging them
}