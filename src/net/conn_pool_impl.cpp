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

#include "conn_pool_impl.hpp"
#include "logger/logger.hpp"
#include "websocket_client_impl.hpp"

namespace mdh {

conn_pool_impl::conn_pool_impl(
    const std::shared_ptr<io_executor>& io_exec,
    const std::shared_ptr<asio::ssl::context>& ctx,
    uint32_t core_id,
    const market_data_config& config
) noexcept : io_exec_(io_exec), config_(config) {
    for (uint32_t i = 0; i < config.workers[core_id].connections.size(); ++i) {
        auto client = std::make_shared<websocket_client_impl>(
            io_exec,
            ctx,
            config.workers[core_id].connections[i].endpoint,
            config.workers[core_id].connections[i].port
        );

        client->open();
        clients_.push_back(std::move(client));
    }
}

conn_pool_impl::~conn_pool_impl() {
    TRACE_LOG("conn_pool_impl destructor");
    for (const auto& client : clients_) {
        client->close();
    }
}

} // namespace mdh
