# Phase 1, 2 & 3: Technical Walkthrough

This document explains the architecture, C++ design choices, and block-by-block logic of the Limit Order Book (LOB) engine as it stands at the end of Phase 3.

---

## 1. Core Data Structures

### `Order` and `OrderRequest` (`order.hpp`)
We separate the **Request** from the **Resting Order**.
- **`OrderRequest`**: A simple POD (Plain Old Data) struct used to pass data into the engine.
- **`Order`**: Contains internal metadata like `sequence` (for Time Priority).
- **Price (`std::int64_t`)**: Transitioned from `double` to integer-based **Ticks**. This avoids floating-point precision issues and speeds up comparisons (Phase 3).

### `PriceLevel` (`order_book.hpp`)
```cpp
struct PriceLevel {
    std::list<Order> orders;
    std::uint32_t totalQuantity;
};
```
- **`std::list`**: In Phase 3, we switched from `std::deque` to `std::list`. While `deque` has better cache locality, `list` allows **O(1) erasure** of any order if we have its iterator.
- **`totalQuantity`**: We cache the sum of all order quantities at this price for $O(1)$ volume lookups.

---

## 2. The Order Book Map

```cpp
using AskLevels = std::map<std::int64_t, PriceLevel>;
using BidLevels = std::map<std::int64_t, PriceLevel, std::greater<std::int64_t>>;
```
- **Sorted Prices**: Using `std::int64_t` for the map keys makes price matching much faster and more deterministic for the CPU compared to `double`.

---

## 3. Matching Logic (`addOrder`)

The matching logic is the "heart" of the engine.

### Block A: Validation & Deduplication
Standard checks for price/quantity validity and duplicate IDs.

### Block B: The Matching Loop
We compare the `incoming.price` to the `bestAskIt->first`. Because these are now integers, the `>` and `<` comparisons are extremely cheap.

### Block C: FIFO Execution
Inside a price level, we always match against the `front()` of the list.

### Block D: Resting & Indexing (Phase 3 Update)
```cpp
if (incoming.quantity > 0){
    PriceLevel& bidLevel = bids_[incoming.price];
    bidLevel.orders.push_back(std::move(incoming));
    
    auto it = std::prev(bidLevel.orders.end()); // Get iterator to the new order
    orderIndex_[it->id] = {it->side, it->price, it}; // Store iterator in the index
}
```
In Phase 3, we now store the **iterator** in our `orderIndex_`. This is the key to the $O(1)$ cancellation optimization.

---

## 4. Phase 3: O(1) Cancellation

### The Optimized Index
```cpp
struct OrderLocator {
    Side side;
    std::int64_t price;
    std::list<Order>::iterator iterator; // Direct pointer to the node
};
```

### Optimized Cancellation Logic
```cpp
bool OrderBook::cancelOrder(std::uint64_t orderId) {
    auto indexIt = orderIndex_.find(orderId);
    if (indexIt == orderIndex_.end()) return false;
    
    OrderLocator locator = indexIt->second;
    PriceLevel& level = (locator.side == Side::Buy) ? bids_[locator.price] : asks_[locator.price];
    
    level.totalQuantity -= locator.iterator->quantity;
    level.orders.erase(locator.iterator); // O(1) Erasure!
    orderIndex_.erase(indexIt);
    
    return true;
}
```
By using the stored iterator, we skip the linear search within the price level. This makes cancellation latency independent of the number of orders in the book.

---

## 5. Performance-Critical C++ Features Used

1.  **Integer Math**: Using Ticks instead of Doubles for zero-latency price comparisons.
2.  **Iterator Stability**: Using `std::list` ensures that iterators remain valid even when other orders are added or removed from the level.
3.  **`std::move`**: Prevents unnecessary copies when moving an order from a request into the resting book.
4.  **`std::unordered_map`**: Provides $O(1)$ average time complexity for looking up orders by ID.

---

## 6. Phase 3 Optimization Summary

| Feature | Phase 2 Complexity | Phase 3 Complexity | Impact |
| :--- | :--- | :--- | :--- |
| **Price Comparison** | Moderate (FP) | **Low (Integer)** | Faster matching loops |
| **Cancellation** | $O(N)$ search | **$O(1)$ direct** | No latency spikes on cancel |
| **Price Precision** | Uncertain | **Perfect** | Reliable matching at all scales |

