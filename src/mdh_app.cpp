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
#include "io/io_executor_impl.hpp"

namespace mdh {
auto mdh_app::init(config& config) noexcept -> void {
    TRACE_LOG("mdh_app::init()");

    auto ssl_ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv12_client);
    ssl_ctx->set_default_verify_paths();

    for(int i = 0; i < config.total_cores; i++){
        INFO_LOG("Core ID: {} starting...", i);

        const auto group = config.symbol_groups[i];
        const auto io_exec_ = std::make_shared<io_executor_impl>(group.core_id);
        auto server = std::make_unique<server_instance>(io_exec_,ssl_ctx,config, group.core_id); 
        servers_map_[group.core_id] = std::move(server);
    }
}

auto mdh_app::run() noexcept -> void {
    TRACE_LOG("mdh_app::run()");

    for (const auto& [core_id, server] : servers_map_) {
        INFO_LOG("Starting server instance on core: {}", core_id);
        server->start();
    }

    std::this_thread::sleep_for(std::chrono::seconds(60));
}

} // namespace mdh