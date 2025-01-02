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
#include "logger/logger.cpp"

int main() {
    std::cout << "Market Data Handler project going to the moon 🚀!" << std::endl;
    logger::init();

    TRACE_LOG("This is a trace log message");
    DEBUG_LOG("This is a debug log message");
    INFO_LOG("This is an info log message");
    WARN_LOG("This is a warning log message");
    ERROR_LOG("This is an error log message");

    return 0;
}