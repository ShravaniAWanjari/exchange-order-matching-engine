#pragma once

#include <cstdint>

namespace lob {
using Price = int64_t;
using Quantity = uint32_t;
using OrderId = uint64_t;

enum class Side : uint8_t { Buy, Sell };

enum class OrderType : uint8_t { Limit, Market };

} // namespace lob