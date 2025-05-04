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

#include <boost/beast/ssl.hpp>
#include "mdh/config/config.hpp"
#include "mdh/io/io_executor.hpp"
#include "mdh/websocket_client.hpp"


namespace mdh {

using message_callback = std::function<void(std::string_view msg)>;

class conn_pool_impl {

public:
    explicit conn_pool_impl(const std::shared_ptr<io_executor>& io_exec, const connection_config conn_config) noexcept;

    ~conn_pool_impl();

    auto set_message_callback(message_callback cb) noexcept -> void;

private:
    auto on_messaget_received(std::string_view msg) noexcept -> void;

    std::shared_ptr<io_executor> io_exec_;
    std::vector<websocket_client_ptr> clients_;

    message_callback message_cb_;
};

} // namespace mdh
