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

#include "mdh/config/config.hpp"
#include "io/io_executor_impl.hpp"
#include "logger/logger.hpp"
#include "net/websocket_client_impl.hpp"
#include "mdh_app.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;

inline auto create_test_config() -> mdh::config {
    mdh::config cfg{
        .total_cores = 10,
        .websockets_per_core = 2,
        .max_symbols_per_connection = 200,
        .reconnect_delay_ms = 1000,
        .symbol_groups = {
            mdh::symbol_group{
                .group = "major_btc",
                .symbols = {
                    "BTCUSDT", "BTCBUSD", "BTCUSDC", 
                    "BTCEUR", "BTCGBP", "BTCAUD"
                },
                .core_id = 0
            },
            mdh::symbol_group{
                .group = "major_eth",
                .symbols = {
                    "ETHUSDT", "ETHBTC", "ETHBUSD", 
                    "ETHEUR", "ETHGBP", "ETHAUD"
                },
                .core_id = 1
            },
            mdh::symbol_group{
                .group = "bnb_pairs",
                .symbols = {
                    "BNBUSDT", "BNBBTC", "BNBETH", 
                    "BNBBUSD", "BNBEUR", "BNBGBP"
                },
                .core_id = 2
            },
            mdh::symbol_group{
                .group = "defi_major",
                .symbols = {
                    "AAVEUSDT", "UNIUSDT", "SUSHIUSDT",
                    "MKRUSDT", "COMPUSDT", "YFIUSDT"
                },
                .core_id = 3
            },
            mdh::symbol_group{
                .group = "layer1",
                .symbols = {
                    "SOLUSDT", "ADAUSDT", "DOTUSDT",
                    "AVAXUSDT", "NEARUSDT", "FTMUSDT"
                },
                .core_id = 4
            },
            mdh::symbol_group{
                .group = "layer2",
                .symbols = {
                    "MATICUSDT", "ARBUSDT", "OPUSDT",
                    "LDOUSDT", "IMXUSDT", "STXUSDT"
                },
                .core_id = 5
            },
            mdh::symbol_group{
                .group = "stables",
                .symbols = {
                    "BUSDUSDT", "USDCUSDT", "EUROUSDT",
                    "GBPUSDT", "AUDUSDT", "DAIUSDT"
                },
                .core_id = 6
            },
            mdh::symbol_group{
                .group = "meme",
                .symbols = {
                    "DOGEUSDT", "SHIBUSDT", "PEPEUSDT",
                    "FLOKIUSDT", "BONKUSDT", "WOJAKUSDT"
                },
                .core_id = 7
            },
            mdh::symbol_group{
                .group = "gaming",
                .symbols = {
                    "SANDUSDT", "MANAUSDT", "AXSUSDT",
                    "GALAUSDT", "ENJUSDT", "CHZUSDT"
                },
                .core_id = 8
            },
            mdh::symbol_group{
                .group = "emerging",
                .symbols = {
                    "INJUSDT", "SUIUSDT", "FETUSDT",
                    "KASTAUSDT", "PYRUSDT", "STRKUSDT"
                },
                .core_id = 9
            }
        }
    };
    
    return cfg;
}

int main() {
    std::cout << "Market Data Handler project going to the moon 🚀!" << std::endl;
    mdh::logger::init();

    mdh::mdh_app app;
    auto config = create_test_config();

    app.init(config);
    
    app.run();

    return 0;
}
