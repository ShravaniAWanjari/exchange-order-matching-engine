#pragma once

#include "../common/order.hpp"
#include <cstddef>
#include <vector>

class OrderPool {
public:
  explicit OrderPool(size_t capacity) {
    pool_.resize(capacity);

    for (Index i = 0; i < capacity - 1; ++i) {
      pool_[i].next = i + 1;
    }

    pool_[capacity - 1].next = INVALID_INDEX;

    first_free_ = 0;
  }

  inline Index allocate() noexcept {
    if (first_free_ == INVALID_INDEX) [[unlikely]] {
      return INVALID_INDEX;
    }

    Index allocated_index = first_free_;
    first_free_ = pool_[first_free_].next;
    return allocated_index;
  }

  inline void deallocate(Index index) noexcept {
    pool_[index].next = first_free_;
    first_free_ = index;
  }

  inline Order &get(Index index) noexcept { return pool_[index]; }

private:
  std::vector<Order> pool_;
  Index first_free_;
};