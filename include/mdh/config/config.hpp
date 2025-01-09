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

#include <string>
#include <vector>


namespace mdh {

struct symbol_group {
    std::string group;
    std::vector<std::string> symbols;
    uint32_t core_id;
};

class config {
public:
    uint32_t total_cores;
    uint32_t websockets_per_core;
    uint32_t max_symbols_per_connection;
    uint32_t reconnect_delay_ms;
    std::vector<symbol_group> symbol_groups;

    static auto from_file(const std::string& path) -> config;
};
} // namespace mdh::config
