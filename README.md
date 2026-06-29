# High-Frequency Trading Order Matching Engine

This project is a low-latency, high-frequency trading (HFT) order matching engine built from scratch in C++.

## Architecture Decisions

### Cross-Platform OS Tuning (Windows vs Linux)
The core business logic (OrderBook, MemoryPool, Matching Engine) is written in **pure, OS-agnostic standard C++**. It does not rely on any OS-specific headers.

However, HFT requires strict thread pinning and OS-scheduler manipulation to guarantee cache locality (L1/L2 cache sharing) and avoid ALU contention. 

- To handle cross-platform differences without maintaining two separate codebases, we use **C++ Preprocessor Macros** (`#ifdef _WIN32` vs `#ifdef __linux__`).
- The thread pinning and networking boundaries will dynamically compile the correct OS-level APIs depending on the host machine.
- **Docker Deployment:** If deployed via Docker, the container behaves as a Linux environment. To achieve true physical core isolation in this scenario, the container must be launched with the `--cpuset-cpus` flag; otherwise, the host OS scheduler will migrate the "pinned" threads across physical cores, negating our cache locality efforts.
