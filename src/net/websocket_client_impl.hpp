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

#include "mdh/websocket_client.hpp"
#include "mdh/io/io_executor.hpp"

namespace websocket = beast::websocket;

namespace mdh {

class websocket_client_impl : public websocket_client, public std::enable_shared_from_this<websocket_client_impl> {

public:
    explicit websocket_client_impl(
        const std::shared_ptr<io_executor>& io_exec,
        const std::shared_ptr<asio::ssl::context>& ctx,
        const std::string& host,
        const std::string& port
    ) noexcept;

    ~websocket_client_impl();

    auto send(std::string_view message) noexcept -> void override;
    auto set_update_handler(update_handler handler) noexcept -> void override;
    auto set_status_handler(status_handler handler) noexcept -> void override;

    auto open() noexcept -> void override;
    auto close() noexcept -> void override;

private:
    auto on_resolse(
        beast::error_code ec,
        asio::ip::tcp::resolver::results_type results
    ) noexcept -> void override;

    auto on_connect(
        beast::error_code ec,
        asio::ip::tcp::resolver::results_type::endpoint_type ep
    ) noexcept -> void override;

    auto on_ssl_handshake(beast::error_code ec) noexcept -> void override;

    auto on_handshake(beast::error_code ec) noexcept -> void override;

    auto on_write(beast::error_code ec, std::size_t bytes_transferred) noexcept -> void override;

    auto on_read(beast::error_code ec, std::size_t bytes_transferred) noexcept -> void override;

    auto on_close(beast::error_code ec) noexcept -> void override;

    const std::shared_ptr<io_executor>& io_executor_;
    const std::shared_ptr<asio::ssl::context>& ssl_ctx_;
    std::string host_;
    std::string port_;
    std::string target_;
    asio::ip::tcp::resolver resolver_;
    beast::flat_buffer buffer_;

    websocket::stream<boost::asio::ssl::stream<beast::tcp_stream>> ws_;


    int last_token = 0;

    update_handler update_handler_;
    status_handler status_handler_;
};

} // namespace mdh
