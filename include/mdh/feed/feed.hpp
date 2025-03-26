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

#include <vector>
namespace mdh {


struct feed {
    std::vector<std::string> symbols;
    std::string websocket_url;
    std::string name;
    
    feed(std::string name, std::string websocket_url, std::vector<std::string> symbols) : name(name), websocket_url(websocket_url), symbols(symbols) {

    }
};


} // namespace mdh
