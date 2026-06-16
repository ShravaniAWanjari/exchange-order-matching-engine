#pragma once

#include "types.hpp"
#include <atomic>

namespace lob {

class OrderIdGenerator {
public:
  inline OrderId next() noexcept {
    return counter_.fetch_add(1, std::memory_order_relaxed);
  }

private:
  std::atomic<OrderId> counter_{1};
};

} // namespace lob
