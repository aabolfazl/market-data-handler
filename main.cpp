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

#include <iostream>

#include "logger/logger.hpp"
#include "feed/binance_feed.hpp"
#include "io/io_executor_impl.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Market Data Handler project going to the moon 🚀!" << std::endl;
    mdh::logger::init();

    auto io_context = std::make_shared<mdh::io_executor_impl>(0);
    mdh::binance_feed binance_feed{io_context};

    binance_feed.subscribe("btcusdt",[](mdh::market_message msg){

    });

    return 0;
}
