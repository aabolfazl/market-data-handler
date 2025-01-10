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

#include <iostream>

#include "logger/logger.hpp"
#include "mdh/config/config.hpp"
#include "mdh_app.hpp"

inline auto create_v1_config() -> mdh::market_data_config {
    const std::vector<std::string> main_pairs = {
        "BTCUSDT", "ETHUSDT", "BNBUSDT", "SOLUSDT", "XRPUSDT",
        "ADAUSDT", "DOGEUSDT", "MATICUSDT", "AVAXUSDT", "LINKUSDT",
        "ETHBTC", "BNBBTC", "SOLBTC", "XRPBTC", "ADABTC",
        "BTCBUSD", "ETHBUSD", "BNBBUSD", "SOLBUSD", "XRPBUSD"
    };

    constexpr uint32_t cores = 2;         
    constexpr uint32_t sockets_per_core = 2;    
    constexpr uint32_t symbols_per_socket = 5;

    std::vector<mdh::worker_config> workers;
    workers.reserve(cores);

    for (uint32_t core_id = 0; core_id < cores; ++core_id) {
        std::vector<mdh::connection_config> connections;
        connections.reserve(sockets_per_core);

        size_t start_idx = core_id * sockets_per_core * symbols_per_socket;

        for (size_t conn_id = 0; conn_id < sockets_per_core; ++conn_id) {
            std::vector<std::string> conn_symbols;
            size_t symbol_start = start_idx + (conn_id * symbols_per_socket);
            for (size_t i = 0; i < symbols_per_socket && symbol_start + i < main_pairs.size(); ++i) {
                conn_symbols.push_back(main_pairs[symbol_start + i]);
            }

            mdh::connection_config conn{
                .symbols = std::move(conn_symbols),
                .streams = {mdh::stream_type::depth, mdh::stream_type::trade},
                .endpoint = "stream.binance.com",
                .port = "443",
                .core_id = core_id,
                .max_message_rate = 5000,
                .auto_reconnect = true,
                .ping_interval_ms = 60 * 1000 * 3,  // 3 minutes
                .reconnect_delay_ms = 1000
            };
            connections.push_back(std::move(conn));
        }

        mdh::worker_config worker{
            .name = fmt::format("worker_{}", core_id),
            .core_id = core_id,
            .connections = std::move(connections),
            .order_book_depth = 20,
            .maintain_full_depth = false,
            .message_queue_size = 1000
        };

        workers.push_back(std::move(worker));
    }

    return mdh::market_data_config{
        .workers = std::move(workers),
        .enable_snapshots = true,
        .snapshot_interval = 1000,
        .pre_allocated_books = 20,
        .memory_pool_size = 1 << 20,
        .enable_metrics = true,
        .metrics_port = 9090
    };
}

int main() {
    std::cout << "Market Data Handler project going to the moon 🚀!" << std::endl;
    mdh::logger::init();

    const auto config = create_v1_config();
    mdh::mdh_app app{config};

    app.init();
    app.run();

    return 0;
}
