# Low Latency LOB Engine

Phase 1 of a C++ limit order book matching engine.

## What it does
- accepts buy and sell limit orders
- matches using price-time priority
- preserves FIFO within each price level
- supports partial fills
- rejects invalid orders
- prints the aggregated final book state

## Build
Requirements:
- CMake 3.16+
- a C++17 compiler

```bash
git clone <your-repo-url>
cd low_latency_lob_engine
cmake -S . -B build
cmake --build build --config Debug
```

## Run
- If you are using the Visual Studio CMake generator on Windows:

```bash
.\build\Debug\lob_engine.exe
```
- If you are using a single-config generator like Ninja or Make:
```bash
./build/lob_engine
```

## Files
- src/order.hpp: order types
- src/order_book.hpp: order book interface
- src/order_book.cpp: matching and book logic
- src/main.cpp: Phase 1 manual test scenarios

## Phase 1
### Implemented:

- core matching engine
- price-time priority
- partial fills
- invalid order rejection
- aggregated book printing
- Not implemented yet:

- cancellation
- order lookup by ID
- benchmarking
- latency optimization

### See also:

- About.md
- Phase1.md

