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

#include "mdh_app.hpp"
#include "logger/logger.hpp"

namespace mdh {
auto mdh_app::init(config& config) noexcept -> void {
    TRACE_LOG("mdh_app::init()");

    for (const auto& group : config.symbol_groups) {
        INFO_LOG("Symbol group: {}", group.group);
        INFO_LOG("Core ID: {}", group.core_id);
        INFO_LOG("Symbols:");
        for (const auto& symbol : group.symbols) {
            INFO_LOG("{}", symbol);
        }
    }
}

auto mdh_app::run() noexcept -> void {
    TRACE_LOG("mdh_app::run()");
}

} // namespace mdh