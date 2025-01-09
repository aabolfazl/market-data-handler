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

namespace mdh {

class mdh_app {
public:
    auto init(config& config) noexcept -> void;
    auto run() noexcept -> void;
};

} // namespace mdh
