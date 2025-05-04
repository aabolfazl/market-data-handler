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
#include <boost/asio/buffer.hpp>
#include "logger/logger.hpp"

namespace mdh {
websocket_client_impl::websocket_client_impl(const std::shared_ptr<io_executor>& io_exec,
                                             const std::shared_ptr<asio::ssl::context>& ssl_ctx,
                                             const std::string& host, const std::string& port) noexcept :
    io_executor_(io_exec),
    ssl_ctx_(ssl_ctx),
    host_(host),
    port_(port),
    resolver_(io_exec->context()),
    ws_(io_exec->context(), *ssl_ctx) {
    TRACE_LOG("websocket_client_impl constructor host: {} port: {}", host_, port_);
}

auto websocket_client_impl::open() noexcept -> void {
    TRACE_LOG("start websocket_client_impl");

    resolver_.async_resolve(
        host_, port_, [self = shared_from_this()](beast::error_code ec, asio::ip::tcp::resolver::results_type results)
        { self->on_resolse(ec, results); });
}

auto websocket_client_impl::on_resolse(beast::error_code ec, asio::ip::tcp::resolver::results_type results) noexcept
    -> void {
    if (ec) {
        ERROR_LOG("on_resolse: {}", ec.message());
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
        return;
    }

    TRACE_LOG("on_resolse");

    beast::get_lowest_layer(ws_).async_connect(
        results,
        [self = shared_from_this()](beast::error_code ec, asio::ip::tcp::resolver::results_type::endpoint_type ep)
        { self->on_connect(ec, ep); });
}

auto websocket_client_impl::on_connect(beast::error_code ec,
                                       asio::ip::tcp::resolver::results_type::endpoint_type ep) noexcept -> void {
    TRACE_LOG("on_connect");

    ws_.next_layer().async_handshake(asio::ssl::stream_base::client,
                                     [self = shared_from_this()](beast::error_code ec) { self->on_ssl_handshake(ec); });
}

auto websocket_client_impl::on_ssl_handshake(beast::error_code ec) noexcept -> void {
    TRACE_LOG("on_ssl_handshake");

    if (ec) {
        ERROR_LOG("on_ssl_handshake: {}", ec.message());
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
        return;
    }

    ws_.async_handshake(host_, "/ws", [self = shared_from_this()](beast::error_code ec) { self->on_handshake(ec); });
}

auto websocket_client_impl::on_handshake(beast::error_code ec) noexcept -> void {
    TRACE_LOG("on_handshake");

    if (ec) {
        ERROR_LOG("on_handshake: {}", ec.message());
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
        return;
    }

    if (status_handler_) {
        status_handler_(event_type::on_connect, shared_from_this());
    }

    ws_.async_read(buffer_, beast::bind_front_handler(&websocket_client_impl::on_read, shared_from_this()));
}

auto websocket_client_impl::send(std::string_view message) noexcept -> void {

    INFO_LOG("request: {}", message);

    ws_.async_write(asio::buffer(message),
                    [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred)
                    { self->on_write(ec, bytes_transferred); });
}

auto websocket_client_impl::on_write(beast::error_code ec, std::size_t bytes_transferred) noexcept -> void {
    TRACE_LOG("send request result {}", bytes_transferred);
};


/*
It is a bad idea to use nlohmann::json as a parameter type for a callback function.
It is better to use std::string_view or std::string.
I will fix this or event remove json in the next versions when I focus on performance.

Also we can use a better way to handle the response from the server.
We can use a buffer to store the response and then parse it.
This way we can reduce the number of allocations and deallocations.

Also I need to implement a buffer pool to reduce the number of allocations and deallocations,
and also reduce the memory fragmentation, and reduce the time spent on memory allocation and deallocation.

*/
auto websocket_client_impl::on_read(beast::error_code ec, std::size_t bytes_transferred) noexcept -> void {
    auto start = std::chrono::high_resolution_clock::now();

    if (ec) {
        ERROR_LOG("on_read: {}", ec.message());
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
        return;
    }

    auto bytes = buffer_.data();
    std::string_view view{reinterpret_cast<const char*>(boost::asio::buffer_sequence_begin(bytes)),
                          boost::asio::buffer_size(bytes)};

    if (update_handler_) {
        update_handler_(view);
    }

    buffer_.consume(bytes_transferred);

    ws_.async_read(buffer_, beast::bind_front_handler(&websocket_client_impl::on_read, shared_from_this()));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    INFO_LOG("on_read duration: {} microseconds {} byte read", duration.count(), bytes_transferred);
}


auto websocket_client_impl::set_update_handler(update_handler handler) noexcept -> void {
    update_handler_ = std::move(handler);
}

auto websocket_client_impl::set_status_handler(status_handler handler) noexcept -> void {
    status_handler_ = std::move(handler);
}

auto websocket_client_impl::close() noexcept -> void {
    TRACE_LOG("close websocket_client_impl on");

    ws_.async_close(websocket::close_code::normal,
                    [self = shared_from_this()](beast::error_code ec) { self->on_close(ec); });
}

auto websocket_client_impl::on_close(beast::error_code ec) noexcept -> void {
    TRACE_LOG("on_close");

    if (ec) {
        ERROR_LOG("on_close: {}", ec.message());
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
        return;
    }
}

websocket_client_impl::~websocket_client_impl() {
    TRACE_LOG("websocket_client_impl destructor");
}

} // namespace mdh
