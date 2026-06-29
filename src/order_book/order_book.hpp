#pragma once
#include "../common/events.hpp"
#include "../common/latency_stats.hpp"
#include "../common/time.hpp"
#include "../common/types.hpp"
#include "order_pool.hpp"
#include "price_level.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>


class OrderBook {

public:
  OrderBook(size_t max_price, size_t max_orders)
      : pool_(max_orders), bids_(max_price), asks_(max_price), best_bid_(0),
        best_ask_(max_price - 1) {
    for (Price p = 0; p < max_price; ++p) {
      bids_[p].price = p;
      asks_[p].price = p;
    }
  }

  template <typename EventQueue>
  void add_order(OrderId id, Price price, Quantity qty, Side side,
                 EventQueue &out_queue) {
    Index new_idx = pool_.allocate();
    if (new_idx == INVALID_INDEX)
      return;

    Order &new_order = pool_.get(new_idx);
    new_order.order_id = id;
    new_order.price = price;
    new_order.quantity = qty;
    new_order.remaining_qty = qty;
    new_order.side = side;
    new_order.next = INVALID_INDEX;
    new_order.prev = INVALID_INDEX;

    if (side == Side::BUY) {
      PriceLevel &level = bids_[price];

      if (level.is_empty()) {
        level.head_order = new_idx;
        level.tail_order = new_idx;
      } else {
        Order &current_tail = pool_.get(level.tail_order);

        current_tail.next = new_idx;
        new_order.prev = level.tail_order;

        level.tail_order = new_idx;
      }

      level.total_quantity += qty;

      best_bid_ = std::max(best_bid_, price);
    } else {
      PriceLevel &level = asks_[price];

      if (level.is_empty()) {
        level.head_order = new_idx;
        level.tail_order = new_idx;
      } else {
        Order &current_tail = pool_.get(level.tail_order);

        current_tail.next = new_idx;
        new_order.prev = level.tail_order;

        level.tail_order = new_idx;
      }
      level.total_quantity += qty;

      best_ask_ = std::min(best_ask_, price);
    }
    out_queue.push(OutboundEvent{EventType::ACK, id, 0, price, qty});
  }

  void cancel_order(Index order_idx) {
    if (order_idx == INVALID_INDEX)
      return;

    Order &order = pool_.get(order_idx);

    PriceLevel &level =
        (order.side == Side::BUY) ? bids_[order.price] : asks_[order.price];

    if (order.prev != INVALID_INDEX) {
      Order &prev_order = pool_.get(order.prev);
      prev_order.next = order.next;
    } else {
      level.head_order = order.next;
    }

    if (order.next != INVALID_INDEX) {
      Order &next_order = pool_.get(order.next);
      next_order.prev = order.prev;
    } else {
      level.tail_order = order.prev;
    }

    level.total_quantity -= order.remaining_qty;

    pool_.deallocate(order_idx);

    if (level.is_empty()) {
      if (order.side == Side::BUY && order.price == best_bid_) {
        while (best_bid_ > 0 && bids_[best_bid_].is_empty()) {
          best_bid_--;
        }
      } else if (order.side == Side::SELL && order.price == best_ask_) {
        while (best_ask_ < asks_.size() - 1 && asks_[best_ask_].is_empty()) {
          best_ask_++;
        }
      }
    }
  }

  template <typename EventQueue>
  Quantity match_order(OrderId taker_id, Price price, Quantity qty, Side side,
                       OrderType type, EventQueue &out_queue) {
    uint64_t start = rdtsc();
    Quantity remaining = qty;

    if (type == OrderType::FOK) {
      Quantity available = 0;
      if (side == Side::BUY) {
        Price temp_ask = best_ask_;
        while (temp_ask <= price && available < qty &&
               temp_ask < asks_.size()) {
          available += asks_[temp_ask].total_quantity;
          temp_ask++;
        }
      } else {
        Price temp_bid = best_bid_;
        while (temp_bid >= price && available < qty && temp_bid > 0) {
          available += bids_[temp_bid].total_quantity;
          temp_bid--;
        }
        if (temp_bid == 0 && available < qty) {
          available += bids_[0].total_quantity;
        }
      }

      if (available < qty) {
        out_queue.push(
            OutboundEvent{EventType::REJECT, taker_id, 0, price, qty});
        return qty;
      }
    }

    if (side == Side::BUY) {
      while (remaining > 0 && best_ask_ <= price) {
        PriceLevel &level = asks_[best_ask_];
        if (level.is_empty())
          break;

        Order &maker_order = pool_.get(level.head_order);
        Quantity match_qty = std::min(remaining, maker_order.remaining_qty);

        out_queue.push(OutboundEvent{EventType::FILL, taker_id,
                                     maker_order.order_id, maker_order.price,
                                     match_qty});
        remaining -= match_qty;
        maker_order.remaining_qty -= match_qty;
        level.total_quantity -= match_qty;

        if (maker_order.remaining_qty == 0) {
          cancel_order(level.head_order);
        }
      }
    } else {
      while (remaining > 0 && best_bid_ >= price) {
        PriceLevel &level = bids_[best_bid_];
        if (level.is_empty())
          break;

        Order &maker_order = pool_.get(level.head_order);
        Quantity match_qty = std::min(remaining, maker_order.remaining_qty);

        out_queue.push(OutboundEvent{EventType::FILL, taker_id,
                                     maker_order.order_id, maker_order.price,
                                     match_qty});

        remaining -= match_qty;
        maker_order.remaining_qty -= match_qty;
        level.total_quantity -= match_qty;

        if (maker_order.remaining_qty == 0) {
          cancel_order(level.head_order);
        }
      }
    }
    if (remaining > 0 && type == OrderType::LIMIT) {
      add_order(taker_id, price, remaining, side, out_queue);
    }
    uint64_t end = rdtsc();
    stats_.record_latency(end - start);
    return remaining;
  }

private:
  OrderPool pool_;
  std::vector<PriceLevel> bids_;
  std::vector<PriceLevel> asks_;

  LatencyHistogram stats_;

  Price best_bid_;
  Price best_ask_;
};