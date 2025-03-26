### Real-time Crypto Market Data Handler Library

Purpose:
- Real-time market data processing library from Exchanges (Binance, Bitfinex, etc.)
- Order book state management
- Market statistics calculation
- Data distribution for trading strategies

Tech Stack:
- C++20
- Boost.Beast/ASIO (WebSocket)
- nlohmann/json
- spdlog
- CMake
- Google Test
- Google Benchmark

Core Features:
1. Market Data Collection
- WebSocket connection management
- Multi-symbol support
- Order book updates
- Trade data processing

2. Data Processing
- Real-time order book maintenance
- Price/volume analytics
- Market depth tracking
- Event-driven architecture

3. Performance Metrics
- < 100μs processing latency
- 10K+ messages/second
- Memory-efficient design
- Low CPU usage
- Scalable architecture
- High availability