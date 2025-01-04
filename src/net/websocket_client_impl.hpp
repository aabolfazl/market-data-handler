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

#include "mdh/websocket_client.hpp"
#include <map>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace mdh {
class websocket_client_impl : public websocket_client, public std::enable_shared_from_this<websocket_client_impl> {

public:
    explicit websocket_client_impl(
        asio::io_context& ioc,
        asio::ssl::context& ctx,
        const std::string& host,
        const std::string& port
    ) noexcept;

    auto send_req(nlohmann::json& request, message_handler handler) noexcept -> void;

    auto start() noexcept -> void;

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

    asio::io_context& ioc_;
    asio::ssl::context& ctx_;
    std::string host_;
    std::string port_;
    std::string target_;
    asio::ip::tcp::resolver resolver_;
    beast::flat_buffer buffer_;

    websocket::stream<boost::asio::ssl::stream<beast::tcp_stream>> ws_;

    std::unordered_map<int, message_handler> on_air_req_map_;

    int last_token = 0;
};

} // namespace mdh
