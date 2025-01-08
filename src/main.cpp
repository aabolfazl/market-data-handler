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

#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <memory>
#include <string>
#include "logger/logger.hpp"

#include "net/websocket_client_impl.hpp"
#include "io/io_executor_impl.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;


int main() {
    std::cout << "Market Data Handler project going to the moon 🚀!" << std::endl;
    mdh::logger::init();

    const std::string host = "stream.binance.com";
    const std::string port = "9443";

    mdh::io_executor_impl io_exec(0);

    asio::ssl::context ssl_ctx(asio::ssl::context::tlsv12_client);
    ssl_ctx.set_default_verify_paths();

    for (int i = 0; i < 100; i++) {
        auto ws_client = std::make_shared<mdh::websocket_client_impl>(io_exec, ssl_ctx, host, port);
        ws_client->start();
    }

    io_exec.start();

    io_exec.join();

    return 0;
}
