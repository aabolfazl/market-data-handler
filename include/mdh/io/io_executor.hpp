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


#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>

namespace mdh {

class io_executor {
public:
    virtual ~io_executor() = default;
    virtual auto start() noexcept -> void = 0;
    virtual auto stop() noexcept -> void = 0;
    virtual auto join() noexcept -> void = 0;

    virtual auto context() noexcept -> boost::asio::io_context& = 0;
    virtual auto running() const noexcept -> bool = 0;
    virtual auto id() const noexcept -> uint = 0;
};

using io_executor_ptr = std::shared_ptr<io_executor>;

} // namespace mdh::io
