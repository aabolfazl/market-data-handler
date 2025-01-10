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

#include <memory>
#include <string>

#include "mdh/io/io_executor.hpp"
#include "mdh/websocket_client.hpp"
#include <boost/beast/ssl.hpp>

namespace asio = boost::asio;


namespace mdh {
class conn_pool_impl {

public:
    explicit conn_pool_impl(
        const std::shared_ptr<io_executor>& io_exec,
        const std::shared_ptr<asio::ssl::context>& ctx,
        const std::string& host,
        const std::string& port,
        uint32_t max_idle_connections,
        uint32_t websockets_per_core
    ) noexcept;

    ~conn_pool_impl();

private:
    std::shared_ptr<io_executor> io_exec_;
    std::vector<websocket_client_ptr> clients_;
};

} // namespace mdh
