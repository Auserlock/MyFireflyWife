//
// Created by arch_ on 24-10-25.
//
#include "Log.h"

std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink;
std::shared_ptr<spdlog::logger> logger;

[[nodiscard]] bool init()
{
    console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/file.log", false);

    console_sink->set_level(spdlog::level::debug);
    file_sink->set_level(spdlog::level::debug);

    logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list { console_sink, file_sink });
    logger->flush_on(spdlog::level::debug);

    set_default_logger(logger);
    logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");

    return true;
}
