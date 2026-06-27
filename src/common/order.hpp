#pragma once
#include "types.hpp"
struct Order {
  OrderId order_id;

  Timestamp timestamp;

  Price price;

  Quantity quantity;

  Quantity remaining_qty;

  Side side;

  OrderType type;
};