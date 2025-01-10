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

#include "io_executor_impl.hpp"
#include "logger/logger.hpp"

namespace mdh {

io_executor_impl::io_executor_impl(
    uint core
) noexcept : core_id_(core), work_guard_(boost::asio::make_work_guard(io_context_)) {}

io_executor_impl::~io_executor_impl() {
    stop();
}

auto io_executor_impl::start() noexcept -> void {
    TRACE_LOG("io_executor_impl::start()");
    if (running_.exchange(true)) {
        return;
    }

    thread_ = std::thread([this] {
        #if defined(__linux__)
            if (core_id_ != -1) {
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(core_id_, &cpuset);
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
            }
        #endif
        io_context_.run();
    });
}

auto io_executor_impl::stop() noexcept -> void {
    TRACE_LOG("io_executor_impl::stop()");
    if (!running_.exchange(false)) {
        return;
    }

    io_context_.stop();
    thread_.join();
}

auto io_executor_impl::context() noexcept -> boost::asio::io_context& {
    return io_context_;
}

auto io_executor_impl::running() const noexcept -> bool{
    return running_.load();
}

auto io_executor_impl::id() const noexcept -> uint {
    return core_id_;
}

auto io_executor_impl::join() noexcept -> void {
    if (thread_.joinable()) {
        thread_.join();
    }
}

}// namespace mdh