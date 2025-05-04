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
#include <string_view>
#include "logger/logger.hpp"
#include "mdh/config/config.hpp"
#include "websocket_client_impl.hpp"

namespace mdh {

conn_pool_impl::conn_pool_impl(
    const std::shared_ptr<io_executor>& io_exec,
    const connection_config conn_config
) noexcept : io_exec_(io_exec) {
    auto ssl_ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv12_client);
    ssl_ctx->set_default_verify_paths();

    for (uint32_t i = 0; i < 10; ++i) {
        auto client = std::make_shared<websocket_client_impl>(
            io_exec,
            ssl_ctx,
            conn_config.endpoint,
            conn_config.port
        );

        client->set_update_handler([&](std::string_view msg) {
            on_messaget_received(msg);
        });

        client->set_status_handler([&](
            event_type type,
            websocket_client_ptr client_ptr
        ) {
            if (type == event_type::on_connect){
                TRACE_LOG("websocket_client_impl status_handler event_type: on_connect");
            }
        });

        client->open();
        clients_.push_back(std::move(client));
    }
}

auto conn_pool_impl::set_message_callback(message_callback cb) noexcept -> void {
    message_cb_ = std::move(cb);
}

auto conn_pool_impl::on_messaget_received(std::string_view msg) noexcept -> void {
    if (message_cb_) {
        message_cb_(msg);
    }
}

conn_pool_impl::~conn_pool_impl() {
    TRACE_LOG("conn_pool_impl destructor");
    for (const auto& client : clients_) {
        client->close();
    }
}

} // namespace mdh
