#pragma once
#include "types.hpp"

enum class EventType : uint8_t { ACK = 0, FILL = 1, CANCEL = 2, REJECT = 3 };

struct OutboundEvent {
  EventType type;
  OrderId maker_order_id;
  OrderId taker_order_id;
  Price price;
  Quantity quantity;
};