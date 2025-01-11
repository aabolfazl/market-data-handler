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
#include "logger/logger.hpp"

namespace mdh {
websocket_client_impl::websocket_client_impl(
    const std::shared_ptr<io_executor>& io_exec,
    const std::shared_ptr<asio::ssl::context>& ssl_ctx,
    const std::string& host,
    const std::string& port
) noexcept : io_executor_(io_exec),
    ssl_ctx_(ssl_ctx),
    host_(host),
    port_(port),
    resolver_(io_exec->context()), ws_(io_exec->context(), *ssl_ctx)
{
    TRACE_LOG("websocket_client_impl constructor host: {} port: {}", host_, port_);
}

auto websocket_client_impl::open() noexcept -> void {
    TRACE_LOG("start websocket_client_impl");

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
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
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

auto websocket_client_impl::send_req(nlohmann::json& request, message_handler handler) noexcept -> void{
    int token = ++last_token;
    request["id"] = std::to_string(token);

    on_air_req_map_[token] = handler;

    auto str = request.dump();

    INFO_LOG("request: {}",str);

    ws_.async_write(
        asio::buffer(str),
        [self = shared_from_this()](
            beast::error_code ec, std::size_t bytes_transferred
        ) {
            self->on_write(ec, bytes_transferred); 
        }
    );
}

auto websocket_client_impl::on_write(
    beast::error_code ec,
    std::size_t bytes_transferred
) noexcept -> void {
    TRACE_LOG("send request result {}", bytes_transferred);
};

auto websocket_client_impl::on_read(
    beast::error_code ec,
    std::size_t bytes_transferred
) noexcept -> void {
    TRACE_LOG("on_read {} bytes_transferred", bytes_transferred);
    auto start = std::chrono::high_resolution_clock::now();

    if (ec) {
        ERROR_LOG("on_read: {}", ec.message());
        if (status_handler_) {
            status_handler_(event_type::on_error, shared_from_this());
        }
        return;
    }

    std::string response(static_cast<char*>(buffer_.data().data()), bytes_transferred);
    buffer_.consume(bytes_transferred);

    auto res_json = nlohmann::json::parse(response);

    if (res_json.contains("id") && !res_json["id"].is_null()) {
        int id = std::stoi(res_json["id"].get<std::string>());
        TRACE_LOG("id is {}",id);
        auto it = on_air_req_map_.find(id);
        if (it != on_air_req_map_.end()) {
            std::string json_str = res_json.dump();
            it->second(json_str);
        } else {
            WARN_LOG("No handler found for ID: {}", id);
        }
    } else {
        if (update_handler_) {
            update_handler_(response);
        }
    }

    ws_.async_read(
        buffer_, 
        beast::bind_front_handler(&websocket_client_impl::on_read, shared_from_this())
    );
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    WARN_LOG("on_read duration: {} microseconds byte read {}", duration.count(),bytes_transferred);
}

auto websocket_client_impl::set_update_handler(update_handler handler) noexcept -> void {
    update_handler_ = std::move(handler);
}

auto websocket_client_impl::set_status_handler(status_handler handler) noexcept -> void {
    status_handler_ = std::move(handler);
}

auto websocket_client_impl::close() noexcept -> void {
    TRACE_LOG("close websocket_client_impl on");
    
    ws_.async_close(
        websocket::close_code::normal, 
        [self = shared_from_this()](
            beast::error_code ec
        ) {
            self->on_close(ec);
        }
    );
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
