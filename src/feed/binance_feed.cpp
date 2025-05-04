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

#include "binance_feed.hpp"
#include "logger/logger.hpp"

namespace mdh {

binance_feed::binance_feed(
    std::shared_ptr<io_executor> io_executor
) noexcept : io_exec_(std::move(io_executor)) {
    assert(io_exec_ && "io_exec_ is null in server_instance constructor");


    connection_config conn{.endpoint = "stream.binance.com",
                           .port = "443",
                           .max_message_rate = 5000,
                           .auto_reconnect = true,
                           .ping_interval_ms = 60 * 1000 * 3,
                           .reconnect_delay_ms = 1000};

    conn_pool_ = std::make_unique<conn_pool_impl>(io_exec_, conn);
    conn_pool_->set_message_callback([&](std::string_view msg) { process_market_message(msg); });

    io_exec_->start();
}

auto binance_feed::process_market_message(std::string_view msg) noexcept -> void {
    ERROR_LOG("process_market_message: {}", msg);

    // if(msg.contains("e") && msg["e"].get<std::string>() == "trade") {
    //     market_message m;
    //     m.symbol = msg["s"].get<std::string>();
    //     m.type = stream_type::trade;
    //     m.timestamp = msg["T"].get<std::uint64_t>();
    //     m.price = msg["p"].get<std::string>();

    //     WARN_LOG("process_market_message trade symbol: {}", m.symbol);
    // } else if(msg.contains("e") && msg["e"].get<std::string>() == "depthUpdate") {
    //     market_message m;
    //     m.symbol = msg["s"].get<std::string>();
    //     m.type = stream_type::depth;

    //     ERROR_LOG("process_market_message depth symbol: {}", m.symbol);
    // }
}

binance_feed::~binance_feed() {
    TRACE_LOG("binance_feed destructor");
}

void binance_feed::subscribe(std::string, std::function<void(market_message)> cb) {

}

} // namespace mdh
