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
#include "server_instance.hpp"

#include <unordered_map>

#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <memory>

#include "mdh/config/config.hpp"

namespace mdh {

class mdh_app {
public:
    auto init(config& config) noexcept -> void;
    auto run() noexcept -> void;

private:
    config config_;

    std::unordered_map<uint32_t, std::unique_ptr<server_instance>> servers_map_;
};

} // namespace mdh
