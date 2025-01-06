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

#include "mdh/io/io_executor.hpp"

namespace mdh {

class io_executor_impl : public io_executor {

    explicit io_executor_impl(uint core) noexcept;
    ~io_executor_impl() override;
    auto start() noexcept -> void override;
    auto stop() noexcept -> void override;
    auto context() noexcept -> boost::asio::io_context& override;

private:
    boost::asio::io_context io_context_;
    std::thread thread_;
    int core_id_;
    std::atomic<bool> running_{false};
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
};

} // namespace mdh
