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
};

class server_instance {
public:
    explicit server_instance(
        std::shared_ptr<io_executor> io_executor,
        std::shared_ptr<asio::ssl::context> ssl_ctx,
        const market_data_config& config,
        uint32_t core_id
    ) noexcept;
    ~server_instance();

    auto start() noexcept -> void;
    auto stop() noexcept -> void;

private:
    auto process_market_message(std::string_view msg) noexcept -> void;


    const market_data_config& config_;
    uint32_t core_id_;
    io_executor_ptr io_exec_;
    std::shared_ptr<asio::ssl::context> ssl_ctx_;
    std::unique_ptr<conn_pool_impl> conn_pool_;
};
} // namespace mdh
