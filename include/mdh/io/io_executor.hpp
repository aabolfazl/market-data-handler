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
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <atomic>

#include "logger/logger.cpp"

namespace asio = boost::asio;

namespace mdh {

class io_executor {
public:
    virtual ~io_executor() = default;
    virtual auto start() noexcept -> void = 0;
    virtual auto stop() noexcept -> void = 0;
    virtual auto context() noexcept -> boost::asio::io_context& = 0;
};


} // namespace mdh::io
