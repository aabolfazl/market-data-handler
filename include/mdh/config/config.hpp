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

enum stream_type {
    depth,
    trade,
};

struct connection_config {
    std::vector<std::string> symbols;
    std::vector<stream_type> streams;
    std::string endpoint;
    std::string port;
    uint32_t core_id;

    uint32_t max_message_rate{5000};
    bool auto_reconnect{true};
    uint32_t ping_interval_ms{60 * 1000 * 3};
    uint32_t reconnect_delay_ms;

    static std::string type_to_string(stream_type type) {
        switch (type) {
        case stream_type::depth:
            return "depth";
        case stream_type::trade:
            return "trade";
        }
        return "unknown";
    }

    static stream_type string_to_type(const std::string& type) {
        if (type == "depth") {
            return stream_type::depth;
        } else if (type == "trade") {
            return stream_type::trade;
        }
        return stream_type::depth;
    }
};

struct worker_config {
    std::string name;
    uint32_t core_id;
    std::vector<connection_config> connections;

    size_t order_book_depth{20};
    bool maintain_full_depth{false};
    size_t message_queue_size{1000};
};

struct market_data_config {
    std::vector<worker_config> workers;

    bool enable_snapshots{true};
    uint32_t snapshot_interval{1000};

    size_t pre_allocated_books{1000};
    size_t memory_pool_size{1 << 20};

    bool enable_metrics{true};
    uint32_t metrics_port{9090};
};
} // namespace mdh
