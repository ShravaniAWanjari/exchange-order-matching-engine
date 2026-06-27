#pragma once
#include <atomic>
#include <types.hpp>

class IDGenerator {
public:
  IDGenerator(const IDGenerator &) = delete;
  IDGenerator &operator=(const IDGenerator &) = delete;

  IDGenerator() = default;

  inline OrderId next() noexcept {
    return counter_.fetch_add(1, std::memory_order_relaxed);
  }

private:
  std::atomic<OrderId> counter_{1};
};
