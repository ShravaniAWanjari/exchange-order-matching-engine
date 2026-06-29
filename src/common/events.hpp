#pragma once
#include "types.hpp"

struct ExecutionReport {
  OrderId maker_order_id;
  OrderId taker_order_id;
  Price match_price;
  Quantity match_quantity;
};