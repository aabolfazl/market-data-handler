/*
 * Copyright © 2024 Vortex project
 *
 * This is the source code of the Vortex project.
 * It is licensed under the MIT License; you should have received a copy
 * of the license in this archive (see LICENSE).
 *
 * Author: Abolfazl Abbasi
 *
 */

#include <benchmark/benchmark.h>
#include <boost/pool/pool.hpp>

struct MarketUpdate {
    std::string symbol = "BTC/USDT";
    double price = 0.0;
    double quantity = 0.0;
    uint64_t timestamp = 0;
};

class MessagePool {
    static constexpr size_t POOL_SIZE = 1024;
    alignas(64) char buffer_[POOL_SIZE * sizeof(MarketUpdate)];
    size_t next_{0};

public:
    MarketUpdate* allocate() {
        if (next_ >= POOL_SIZE) {
            next_ = 0;
        }
        auto* ptr = reinterpret_cast<MarketUpdate*>(&buffer_[next_++ * sizeof(MarketUpdate)]);

        return new (ptr) MarketUpdate{};
    }
};

static void BM_HeapAllocation(benchmark::State& state) {
    for (auto _ : state) {
        auto* update = new MarketUpdate{};
        benchmark::DoNotOptimize(update);
        delete update;
    }
}
BENCHMARK(BM_HeapAllocation);

static void BM_CustomPool(benchmark::State& state) {
    MessagePool pool;
    for (auto _ : state) {
        auto* update = pool.allocate();
        benchmark::DoNotOptimize(update);
    }
}
BENCHMARK(BM_CustomPool);

static void BM_BoostPool(benchmark::State& state) {
    boost::pool<> pool(sizeof(MarketUpdate));
    pool.set_max_size(1024);
    for (auto _ : state) {
        auto* update = static_cast<MarketUpdate*>(pool.malloc());
        benchmark::DoNotOptimize(update);
        pool.free(update);
    }
}
BENCHMARK(BM_BoostPool);

BENCHMARK_MAIN();
