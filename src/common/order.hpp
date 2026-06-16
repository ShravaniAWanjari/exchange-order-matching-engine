#pragma once

#include "types.hpp"

namespace lob {
struct alignas(32) Order {
  OrderId id;
  Price price;
  uint64_t timestamp;
  Quantity quantity;
  Side side;
  OrderType type;
};
} // namespace lob