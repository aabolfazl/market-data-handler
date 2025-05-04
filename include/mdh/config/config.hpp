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
#include <cstdint>

namespace mdh {

enum stream_type {
    depth,
    trade,
};

struct connection_config {
    std::string endpoint;
    std::string port;

    uint32_t max_message_rate{5000};
    bool auto_reconnect{true};
    uint32_t ping_interval_ms{60 * 1000 * 3};
    uint32_t reconnect_delay_ms;

};

struct worker_config {
    std::string name;
    uint32_t core_id;

};

struct market_data_config {
    bool enable_metrics{true};
    uint32_t metrics_port{9090};
};
} // namespace mdh
