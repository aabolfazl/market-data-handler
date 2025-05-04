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

#include "mdh/config/config.hpp"
#include "mdh/io/io_executor.hpp"
#include "net/conn_pool_impl.hpp"

#include <memory>

namespace mdh {

struct market_message {
    std::string_view data;
    std::string_view symbol;
    mdh::stream_type type;
    std::uint64_t timestamp;
    std::string_view price;
};

class binance_feed {
public:
    explicit binance_feed(std::shared_ptr<io_executor> io_executor) noexcept;

    ~binance_feed();

    void subscribe(std::string, std::function<void(market_message)> cb);

private:
    auto process_market_message(std::string_view msg) noexcept -> void;

    io_executor_ptr io_exec_;
    std::unique_ptr<conn_pool_impl> conn_pool_;
};
} // namespace mdh
