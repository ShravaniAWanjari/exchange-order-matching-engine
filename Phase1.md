# Phase 1 Plan

## Goal

Build a clean, correct limit order book engine before adding extra features or optimization work.

This phase is only for:

1. representing orders correctly
2. storing bids and asks correctly
3. matching orders with price-time priority
4. producing deterministic output for simple scenarios

---

## Recommended File Structure

```text
low_latency_lob_engine/
|
|-- CMakeLists.txt
|-- README.md
|-- About.md
|-- Phase1.md
|
|-- src/
|   |-- main.cpp
|   |-- order.hpp
|   |-- order_book.hpp
|   |-- order_book.cpp
|   |-- matcher.hpp
|   |-- matcher.cpp
|   |-- parser.hpp
|   |-- parser.cpp
```

This is intentionally small. Do not add more files unless a real need appears.

---

## What Goes Where

### `src/order.hpp`

Owns the core domain types.

This file should define things like:

1. `Side`
2. `Order`
3. possibly `Trade` if you want matching to return execution records

This file should answer:

1. what is an order
2. what fields are required
3. what is the internal representation used by the engine

Keep it simple and stable.

---

### `src/order_book.hpp`

Declares the `OrderBook` type and its public interface.

This file should answer:

1. what operations the book supports
2. what state the book owns
3. what methods outside code is allowed to call

Typical responsibilities:

1. hold bids and asks
2. expose functions for adding orders
3. expose functions for viewing book state

This is the "owner of state" file.

---

### `src/order_book.cpp`

Implements the storage and maintenance behavior of the book.

This file should handle:

1. inserting resting orders
2. removing filled orders
3. removing empty price levels
4. maintaining invariants of the container structure

This file should not become a dumping ground for parsing or demo code.

---

### `src/matcher.hpp`

Declares matching-related functions or a matcher class.

This file should answer:

1. how matching is invoked
2. what matching returns
3. what inputs matching needs

Its role is to separate the matching procedure from unrelated concerns.

---

### `src/matcher.cpp`

Implements the actual matching logic.

This is where the engine should do things like:

1. compare incoming buy against best asks
2. compare incoming sell against best bids
3. execute partial fills
4. update quantities
5. emit trade records if you include them in Phase 1

This is the "engine logic" file.

---

### `src/parser.hpp`

Declares how raw input is converted into valid internal data.

This file should answer:

1. what input format your demo driver accepts
2. what validation happens before an order enters the engine

Keep parser responsibility narrow.

It should validate and translate, not own matching rules.

---

### `src/parser.cpp`

Implements input cleaning and conversion.

Examples of responsibilities:

1. convert raw text into `Order`
2. reject invalid side values
3. reject invalid quantity or price values
4. ensure types are clean before entering the engine

If parsing feels too heavy for early Phase 1, you can delay this and manually construct orders in `main.cpp` first.

That is often the better learning path.

---

### `src/main.cpp`

This is the driver for Phase 1.

It should:

1. create an order book
2. create or parse sample orders
3. pass them into the engine
4. print outcomes or final book state

This file should stay small.

Its job is orchestration, not business logic.

---

## Recommended Build Order

Do not build everything at once.

Build in this order:

1. `order.hpp`
2. `order_book.hpp`
3. `order_book.cpp`
4. `matcher.hpp`
5. `matcher.cpp`
6. `main.cpp`
7. `parser.hpp`
8. `parser.cpp`

Reason:

The engine can work without a parser at first. Parsing is useful, but it is not the hardest part and should not block you from learning the core data flow.

---

## Runtime Flow

The Phase 1 execution flow should look like this:

```text
raw input or manual test order
        |
        v
parser / validation
        |
        v
valid Order object
        |
        v
OrderBook receives incoming order
        |
        v
matching logic checks opposite side
        |
        +--> if match exists:
        |        execute fills
        |        reduce quantities
        |        remove filled resting orders
        |        continue while match is possible
        |
        +--> if remaining quantity still exists:
                 place leftover into the book
        |
        v
updated book state + optional trade records
```

---

## Responsibility Flow

Think of the files like this:

1. `order.hpp` = data definition
2. `parser.*` = raw input to valid domain object
3. `matcher.*` = execution logic
4. `order_book.*` = state ownership and container maintenance
5. `main.cpp` = demonstration and manual testing

That is a cleaner mental model than "one script for everything."

---

## Phase 1 Invariants

Before writing code, keep these rules fixed:

1. bids are sorted best price first
2. asks are sorted best price first
3. within a price level, orders are FIFO
4. a trade only happens when prices cross
5. partially filled orders remain with reduced quantity
6. fully filled orders are removed
7. empty price levels are removed

If these invariants are preserved, the engine is probably structurally correct.

---

## Practical Advice

A good first version of Phase 1 does not need:

1. cancellation
2. order lookup by ID
3. benchmarks
4. custom allocators
5. fancy abstractions

A good first version does need:

1. clear naming
2. deterministic behavior
3. hand-tested examples
4. small, understandable files

---

## Best Starting Point

If you want the least friction, start with:

1. `order.hpp`
2. `order_book.hpp`
3. `order_book.cpp`
4. `main.cpp`

Then once you understand the basic engine, split out `matcher.*` and `parser.*` if that separation feels useful.

That is closer to how a professional would keep early complexity under control.
