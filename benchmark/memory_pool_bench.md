# Memory Pool Benchmark Results

## Overview
This document contains benchmark results for the Market Data Handler's memory pool implementation.

## Results

### Benchmark Metrics
- Allocation Speed
- Memory Usage
- Deallocation Performance

> Note: Detailed benchmark results will be added after running performance tests.

### Run Benchmarks
```bash
$ cmake ..
$ make
$ ./bin/memory_pool_bench
```

## Benchmark Results
```bash
Running ./bin/memory_pool_bench
Run on (10 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 1.94, 2.13, 2.01
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
BM_HeapAllocation       19.3 ns         19.3 ns     36899785
BM_CustomPool           1.04 ns         1.03 ns    673977720
BM_BoostPool            5.00 ns         4.99 ns    138800761
```

## Analysis
- The custom memory pool implementation is faster than the heap allocation and Boost pool.