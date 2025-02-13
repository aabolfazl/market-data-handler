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
$ build % ./bin/smart_ptr_bench
```

## Benchmark Results
```bash
This does not affect benchmark measurements, only the metadata output.
Running ./bin/smart_ptr_bench
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 4.54, 3.71, 3.52
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
BM_HeapAllocation       20.2 ns         20.1 ns     34944613
BM_CustomPool           19.4 ns         19.1 ns     37109686
BM_BoostPool            5.16 ns         5.09 ns    137575912
```

## Analysis
Benchmark analysis and comparisons will be documented in this section.