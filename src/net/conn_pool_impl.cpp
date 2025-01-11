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
#include "logger/logger.hpp"
#include "websocket_client_impl.hpp"

namespace mdh {

conn_pool_impl::conn_pool_impl(
    const std::shared_ptr<io_executor>& io_exec,
    const std::shared_ptr<asio::ssl::context>& ctx,
    uint32_t core_id,
    const market_data_config& config
) noexcept : io_exec_(io_exec), config_(config) {
    const auto& worker_config = config.workers[core_id];
    clients_.reserve(worker_config.connections.size());

    for (uint32_t i = 0; i < config.workers[core_id].connections.size(); ++i) {
        auto client = std::make_shared<websocket_client_impl>(
            io_exec,
            ctx,
            config.workers[core_id].connections[i].endpoint,
            config.workers[core_id].connections[i].port
        );

        client->open();
        client->set_update_handler([&](std::string_view msg) {
            on_messaget_received(msg);
        });

        client->set_status_handler([&](
            event_type type,
            websocket_client_ptr client_ptr
        ) {
            if (type == event_type::on_connect){
                TRACE_LOG("websocket_client_impl status_handler event_type: on_connect");
                
                auto data = nlohmann::json {
                    {"method", "SUBSCRIBE"},
                    {"params", {"btcusdt@aggTrade", "btcusdt@depth","btcusdt@trade","etcusdt@aggTrade", "etcusdt@depth","etcusdt@trade"}},
                    {"id", 1}
                };

                client_ptr->send_req(
                    data,[](
                    std::string_view p
                ){
                    ERROR_LOG("subscribtion reqeust send {}", p);
                });
            }
        });

        clients_.push_back(std::move(client));
    }
}

auto conn_pool_impl::set_message_callback(message_callback cb) noexcept -> void {
    message_cb_ = std::move(cb);
}

auto conn_pool_impl::on_messaget_received(std::string_view msg) noexcept -> void {
    TRACE_LOG("conn_pool_impl message received {}", msg.length());

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
