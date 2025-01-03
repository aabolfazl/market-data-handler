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

#include "websocket_client_impl.hpp"
#include "logger/logger.cpp"

namespace mdh {
websocket_client_impl::websocket_client_impl(
    asio::io_context& ioc,
    asio::ssl::context& ctx,
    const std::string& host,
    const std::string& port
) : ioc_(ioc), ctx_(ctx), host_(host), port_(port), resolver_(ioc), ws_(ioc, ctx) {
    TRACE_LOG("websocket_client_impl constructor");
}

auto websocket_client_impl::start() noexcept -> void {
    TRACE_LOG("start");

    resolver_.async_resolve(
        host_, port_,
        [self = shared_from_this()](
            beast::error_code ec,
            asio::ip::tcp::resolver::results_type results
        ) {
            self->on_resolse(ec, results);
        }
    );
}

auto websocket_client_impl::on_resolse(
    beast::error_code ec,
    asio::ip::tcp::resolver::results_type results
) noexcept -> void {
    TRACE_LOG("on_resolse");

    if (ec) {
        ERROR_LOG("on_resolse: {}", ec.message());
        return;
    }

    beast::get_lowest_layer(ws_).async_connect(
        results,
        [self = shared_from_this()](
            beast::error_code ec,
            asio::ip::tcp::resolver::results_type::endpoint_type ep
        ) {
            self->on_connect(ec, ep);
        }
    );
}

auto websocket_client_impl::on_connect(beast::error_code ec, asio::ip::tcp::resolver::results_type::endpoint_type ep) noexcept -> void {
    TRACE_LOG("on_connect");

    ws_.next_layer().async_handshake(
        asio::ssl::stream_base::client,
        [self = shared_from_this()](beast::error_code ec) {
             self->on_ssl_handshake(ec); 
        }
    );
}

auto websocket_client_impl::on_ssl_handshake(beast::error_code ec) noexcept -> void {
    TRACE_LOG("on_ssl_handshake");

    if (ec) {
        ERROR_LOG("on_ssl_handshake: {}", ec.message());
        return;
    }

    ws_.async_handshake(
        host_, 
        "/ws", 
        [self = shared_from_this()](
            beast::error_code ec
        ) { 
            self->on_handshake(ec); 
        }
    );
}

auto websocket_client_impl::on_handshake(beast::error_code ec) noexcept -> void {
    TRACE_LOG("on_handshake");

    if (ec) {
        ERROR_LOG("on_handshake: {}", ec.message());
        return;
    }

    ws_.async_read(buffer_, beast::bind_front_handler(&websocket_client_impl::on_read, shared_from_this()));
}

auto websocket_client_impl::on_write(
    beast::error_code ec,
    std::size_t bytes_transferred
) noexcept -> void {};

auto websocket_client_impl::on_read(
    beast::error_code ec,
    std::size_t bytes_transferred
) noexcept -> void {
    TRACE_LOG("on_read {}", bytes_transferred);

    if (ec) {
        ERROR_LOG("on_read: {}", ec.message());
        return;
    }

    buffer_.consume(bytes_transferred);

    ws_.async_read(buffer_, beast::bind_front_handler(&websocket_client_impl::on_read, shared_from_this()));
}

auto websocket_client_impl::on_close(beast::error_code ec) noexcept -> void {}

} // namespace mdh
