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

#include <boost/beast/ssl.hpp>
#include "mdh/config/config.hpp"
#include "mdh/io/io_executor.hpp"
#include "mdh/websocket_client.hpp"

namespace asio = boost::asio;

namespace mdh {

using message_callback = std::function<void(nlohmann::json&)>;

class conn_pool_impl {

public:
    explicit conn_pool_impl(
        const std::shared_ptr<io_executor>& io_exec,
        const std::shared_ptr<asio::ssl::context>& ctx,
        uint32_t core_id,
        const market_data_config& config
    ) noexcept;

    ~conn_pool_impl();

    auto set_message_callback(message_callback cb) noexcept -> void;

private:
    auto on_messaget_received(nlohmann::json& msg) noexcept -> void;

    std::shared_ptr<io_executor> io_exec_;
    std::vector<websocket_client_ptr> clients_;
    const market_data_config& config_;
    message_callback message_cb_;
};

} // namespace mdh
