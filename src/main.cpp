#include "common/events.hpp"
#include "common/types.hpp"
#include "order_book/order_book.hpp"
#include <iostream>
#include <vector>

struct MockEventQueue {
  std::vector<OutboundEvent> events;
  inline void push(const OutboundEvent &event) noexcept {
    events.push_back(event);
  }
};

void print_event(const OutboundEvent &event) {
  std::cout << "[Event] Type: ";
  switch (event.type) {
  case EventType::ACK:
    std::cout << "ACK";
    break;
  case EventType::FILL:
    std::cout << "FILL";
    break;
  case EventType::CANCEL:
    std::cout << "CANCEL";
    break;
  case EventType::REJECT:
    std::cout << "REJECT";
    break;
  }
  std::cout << " | TakerID: " << event.taker_order_id
            << " | MakerID: " << event.maker_order_id
            << " | Price: " << event.price << " | Qty: " << event.quantity
            << std::endl;
}

int main() {
  std::cout << "=== HFT MATCHING ENGINE Test Harness === " << std::endl;

  OrderBook book(1000, 100);
  MockEventQueue queue;

  std::cout << "\nInserting resting SELL limit order at price 100, qty 10..."
            << std::endl;
  book.match_order(1001, 100, 10, Side::SELL, OrderType::LIMIT, queue);

  std::cout << "\nInserting matching BUY limit order at price 100, qty 10..."
            << std::endl;
  book.match_order(1002, 100, 10, Side::BUY, OrderType::LIMIT, queue);

  std::cout << "\nInserting order with price 1001 (exceeds max book price of "
               "1000) (should REJECT) ..."
            << std::endl;
  book.match_order(1003, 1001, 10, Side::BUY, OrderType::LIMIT, queue);

  std::cout << "\nInserting duplicate Order ID 1002 (Should REJECT)..."
            << std::endl;
  book.match_order(1002, 100, 10, Side::BUY, OrderType::LIMIT, queue);

  std::cout << "\nGenerated Events:" << std::endl;
  for (const auto &event : queue.events) {
    print_event(event);
  }

  return 0;
}