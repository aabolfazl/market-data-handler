/*
 * Copyright © 2025 Market Data Handler project
 *
 * This is the source code of the Market Data Handler project.
 * It is licensed under the MIT License; you should have received a copy
 * of the license in this archive (see LICENSE).
 *
 * Author: Abolfazl Abbasi
 *
 */

#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include "define.hpp"

namespace mdh {

#ifdef ULTRA_LOW_LATENCY_MODE
#define TRACE_LOG(...)
#define DEBUG_LOG(...)
#define INFO_LOG(...)
#define WARN_LOG(...) spdlog::warn(__VA_ARGS__)
#define ERROR_LOG(...) spdlog::error(__VA_ARGS__)
#else
#define TRACE_LOG(...) spdlog::trace(__VA_ARGS__)
#define DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#define INFO_LOG(...) spdlog::info(__VA_ARGS__)
#define WARN_LOG(...) spdlog::warn(__VA_ARGS__)
#define ERROR_LOG(...) spdlog::error(__VA_ARGS__)
#endif

class logger {
public:
    static void init() {
        auto console = spdlog::stdout_color_mt("console");
        spdlog::set_default_logger(console);
        spdlog::set_pattern("%^[%H:%M:%S.%e][%l]%$ %v");

#ifdef ULTRA_LOW_LATENCY_MODE
        spdlog::set_level(spdlog::level::warn);
#else
        spdlog::set_level(spdlog::level::trace);
#endif
    }
};

} // namespace mdh