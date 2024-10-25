//
// Created by arch_ on 24-10-25.
//
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

extern std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
extern std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink;
extern std::shared_ptr<spdlog::logger> logger;

[[nodiscard]] bool init();
