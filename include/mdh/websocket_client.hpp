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

#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

#include <memory>

namespace beast = boost::beast;
namespace asio = boost::asio;

namespace mdh {

using message_handler = std::function<void(std::string_view)>;
using update_handler = std::function<void(std::string_view)>;

class websocket_client {

public:

    virtual auto open() noexcept -> void = 0;
    
    virtual auto close() noexcept -> void = 0;

    virtual auto on_resolse(
        beast::error_code ec,
        asio::ip::tcp::resolver::results_type results
    ) noexcept -> void = 0;

    virtual auto on_connect(
        beast::error_code ec,
        asio::ip::tcp::resolver::results_type::endpoint_type ep
    ) noexcept -> void = 0;

    virtual auto on_ssl_handshake(beast::error_code ec) noexcept -> void = 0;

    virtual auto on_handshake(beast::error_code ec) noexcept -> void = 0;

    virtual auto on_write(beast::error_code ec, std::size_t bytes_transferred) noexcept -> void = 0;

    virtual auto on_read(beast::error_code ec, std::size_t bytes_transferred) noexcept -> void = 0;

    virtual auto on_close(beast::error_code ec) noexcept -> void = 0;
};

// I am using shared_ptr because i want to use shared_from_this() in websocket_client_impl, its not high performance and I will use in phase 2
using websocket_client_ptr = std::shared_ptr<websocket_client>;

} // namespace mdh
