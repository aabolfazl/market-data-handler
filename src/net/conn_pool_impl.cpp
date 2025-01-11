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

    for (uint32_t i = 0; i < worker_config.connections.size(); ++i) {
        auto& conn_config = worker_config.connections[i];

        auto client = std::make_shared<websocket_client_impl>(
            io_exec,
            ctx,
            conn_config.endpoint,
            conn_config.port
        );

        client->set_update_handler([&](std::string_view msg) {
            on_messaget_received(msg);
        });

        auto watch_list = std::vector<std::string>{};
        watch_list.reserve(conn_config.symbols.size() * conn_config.streams.size());

        for (const auto& symbol : conn_config.symbols) {
            for (const auto& stream : conn_config.streams) {
                watch_list.push_back(symbol + "@" + connection_config::type_to_string(stream));
            }
        }

        client->set_status_handler([&, watch_list](
            event_type type,
            websocket_client_ptr client_ptr
        ) {
            if (type == event_type::on_connect){
                TRACE_LOG("websocket_client_impl status_handler event_type: on_connect");
                
                auto data = nlohmann::json {
                    {"method", "SUBSCRIBE"},
                    {"params", watch_list},
                    {"id", 1}
                };

                client_ptr->send_req(data,[](
                    std::string_view p
                ){
                    ERROR_LOG("subscribtion reqeust send {}", p);
                });
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
