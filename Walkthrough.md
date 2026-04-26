# Phase 1 & 2: Technical Walkthrough

This document explains the architecture, C++ design choices, and block-by-block logic of the Limit Order Book (LOB) engine as it stands at the end of Phase 2.

---

## 1. Core Data Structures

### `Order` and `OrderRequest` (`order.hpp`)
We separate the **Request** from the **Resting Order**.
- **`OrderRequest`**: A simple POD (Plain Old Data) struct used to pass data into the engine.
- **`Order`**: Contains internal metadata like `sequence` (for Time Priority). Using `std::uint64_t` for IDs and sequences ensures we don't overflow in high-throughput systems.

### `PriceLevel` (`order_book.hpp`)
```cpp
struct PriceLevel {
    std::deque<Order> orders;
    std::uint32_t totalQuantity;
};
```
- **`std::deque`**: Chosen for Phase 1/2 because it allows $O(1)$ push to back and pop from front. Unlike `std::vector`, it doesn't require reallocating the entire buffer when it grows, which keeps latency more predictable (though not perfectly).
- **`totalQuantity`**: We cache the sum of all order quantities at this price. This makes "Aggregated Volume" lookups $O(1)$ instead of $O(N)$.

---

## 2. The Order Book Map

```cpp
using AskLevels = std::map<double, PriceLevel>;
using BidLevels = std::map<double, PriceLevel, std::greater<double>>;
```
- **`std::map`**: A Red-Black Tree. It keeps prices sorted.
- **Price Priority**: 
    - `AskLevels` (Sells) are sorted ascending (lowest price first).
    - `BidLevels` (Buys) are sorted descending (highest price first) using `std::greater`.
- **Complexity**: Lookup and insertion are $O(\log L)$ where $L$ is the number of price levels.

---

## 3. Matching Logic (`addOrder`)

The matching logic is the "heart" of the engine. Here is the block-by-block breakdown:

### Block A: Validation & Deduplication
```cpp
if (request.price <= 0.0 || request.quantity == 0) return;
if (orderIndex_.find(request.id) != orderIndex_.end()) return;
```
Before matching, we ensure the order is valid and that the ID isn't already in the book.

### Block B: The Matching Loop
```cpp
while(incoming.quantity > 0 && !asks_.empty()){
    auto bestAskIt = asks_.begin(); // Lowest Sell Price
    if (bestAskIt->first > incoming.price) break; // No more crossing prices
    
    PriceLevel& askLevel = bestAskIt->second;
    // ... Match against orders in this level ...
}
```
We compare the `incoming.price` to the `bestAskIt->first`. If a Buy order price is $\ge$ the lowest Sell price, a match occurs.

### Block C: FIFO Execution
```cpp
while (incoming.quantity > 0 && !askLevel.orders.empty()) {
    Order& restingAsk = askLevel.orders.front();
    uint32_t tradedQuantity = std::min(incoming.quantity, restingAsk.quantity);
    
    incoming.quantity -= tradedQuantity;
    restingAsk.quantity -= tradedQuantity;
    // ...
}
```
Inside a price level, we always match against the `front()` of the `deque`. This implements **Time Priority (FIFO)**.

### Block D: Resting the Remainder
```cpp
if (incoming.quantity > 0){
    PriceLevel& bidLevel = bids_[incoming.price];
    bidLevel.orders.push_back(std::move(incoming));
}
```
If the order isn't fully filled, we "park" it in the book. Using `std::move` avoids a deep copy of the `Order` object, which is a micro-optimization for speed.

---

## 4. Phase 2: The Index & Cancellation

### The Order Index
```cpp
std::unordered_map<std::uint64_t, OrderLocator> orderIndex_;
```
In Phase 1, canceling an order would require searching every price level ($O(L \times N)$). 
In Phase 2, we added `orderIndex_`. It tells us exactly which `Side` and `Price` an order ID belongs to. This makes the lookup $O(1)$.

### Cancellation Logic
```cpp
bool OrderBook::cancelOrder(std::uint64_t orderId) {
    auto indexIt = orderIndex_.find(orderId);
    if (indexIt == orderIndex_.end()) return false;
    
    OrderLocator locator = indexIt->second;
    // Go directly to the correct side and price level
    auto levelIt = bids_.find(locator.price); 
    // ... Search and remove from deque ...
}
```
Even though we find the price level in $O(\log L)$, we still perform a linear search in the `deque` to remove the order. This is $O(N)$ at the price level. **Phase 3 will optimize this to $O(1)$.**

---

## 5. Performance-Critical C++ Features Used

1.  **Header/Source Separation**: Compiling `order_book.cpp` separately reduces build times and keeps the interface clean.
2.  **`std::uint64_t`**: Prevents "Y2K" style bugs with order IDs and sequences.
3.  **`const` correctness**: Methods like `snapshot()` are marked `const` to guarantee they don't modify the book, allowing the compiler to optimize better.
4.  **`std::reserve()`**: Used in `snapshot()` to prevent multiple reallocations of the return vectors.
5.  **Passing by Reference (`&`)**: We pass `OrderRequest` by `const reference` to avoid copying the struct on every function call.
