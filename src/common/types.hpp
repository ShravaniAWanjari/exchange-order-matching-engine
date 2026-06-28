#pragma once
#include <cstdint>
#include <limits>

enum class Side : uint8_t { BUY = 0, SELL = 1 };

enum class OrderType : uint8_t { LIMIT = 0, MARKET = 1, IOC = 2, FOK = 3 };

using Price = uint64_t;

using Quantity = uint32_t;

using OrderId = uint64_t;

using Timestamp = uint64_t;

using Index = uint32_t;
constexpr Index INVALID_INDEX = std::numeric_limits<Index>::max();