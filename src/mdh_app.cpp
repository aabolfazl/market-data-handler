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

mdh_app::mdh_app(const market_data_config& config) noexcept : config_(config) {
    TRACE_LOG("mdh_app constructor");
}

auto mdh_app::init() noexcept -> void {
    TRACE_LOG("mdh_app::init()");

    auto ssl_ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv12_client);
    ssl_ctx->set_default_verify_paths();

    for (auto worker : config_.workers) {
        INFO_LOG("Core ID: {} starting...", worker.core_id);

        const auto io_exec_ = std::make_shared<io_executor_impl>(worker.core_id);
        auto server = std::make_unique<server_instance>(io_exec_, ssl_ctx, config_, worker.core_id);
        servers_map_[worker.core_id] = std::move(server);
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