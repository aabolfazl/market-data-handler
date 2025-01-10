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
    const std::string& host,
    const std::string& port,
    uint32_t max_idle_connections,
    uint32_t websockets_per_core
) noexcept : io_exec_(io_exec) {
    for (uint32_t i = 0; i < websockets_per_core; ++i) {
        auto client = std::make_shared<websocket_client_impl>(
            io_exec,
            ctx,
            host,
            port
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
