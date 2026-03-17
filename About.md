# Low Latency LOB Engine

## Purpose

This repository is being rebuilt from zero.

The goal is not to generate a project quickly. The goal is to learn how to build a serious C++ systems project by writing it line by line, understanding every design choice, and developing the ability to implement without depending on generated code.

This project should eventually become:

1. a strong C++ learning vehicle
2. a serious systems project for resume and interviews
3. a foundation for lower-latency trading infrastructure work

---

## End State

The final repository should contain a high-quality limit order book matching engine that:

1. is correct
2. is benchmarked
3. is explainable
4. is optimized in later stages
5. is usable by other people who clone the repo

The target is not "toy code." The target is a repository that shows systems thinking, clean structure, and performance awareness.

---

## Three Phases

### Phase 1: Working Engine

Build a clean, correct matching engine first.

This phase is only about correctness, clarity, and learning the fundamentals of C++ through a real project.

#### Phase 1 objectives

1. Represent orders cleanly
2. Build bid and ask books
3. Implement price-time priority
4. Support partial fills
5. Remove fully matched orders correctly
6. Keep code readable and deterministic
7. Build and run from the command line with CMake

#### Phase 1 mindset

Do not optimize early.

The point is to understand:

1. classes and structs
2. header/source separation
3. references and copies
4. stack vs heap behavior
5. STL containers
6. control flow in matching logic

#### Phase 1 deliverable

A working engine that accepts orders, matches them correctly, and can be demonstrated with simple scenarios.

---

### Phase 2: Feature Expansion

After the engine works, add the features that make it more realistic and easier to analyze.

#### Phase 2 objectives

1. Order cancellation by ID
2. Order lookup structures
3. Book printing / snapshot output
4. Aggregated volume per price level
5. Benchmark driver
6. Better test coverage
7. Cleaner APIs for external use

#### Phase 2 mindset

This phase is about turning the engine from "works" into "usable and inspectable."

The point is to learn:

1. interface design
2. auxiliary data structures
3. invariants across multiple containers
4. how features affect complexity and memory layout

#### Phase 2 deliverable

A feature-complete engine with tooling around it, not just core matching.

---

### Phase 3: Latency Optimization

Only after correctness and feature completeness are solid should optimization begin.

#### Phase 3 objectives

1. Measure performance before changing architecture
2. Reduce unnecessary allocations
3. Reduce pointer chasing
4. Revisit container choices
5. Improve cache behavior
6. Explore custom allocators / pools
7. Explore lower-latency data layouts
8. Tighten benchmarks and profiling workflow

#### Phase 3 mindset

Optimization must be justified by measurement.

This phase is about learning:

1. data-oriented design
2. cache-aware programming
3. allocator impact
4. predictable latency vs average throughput
5. tradeoffs between elegant code and fast code

#### Phase 3 deliverable

A much stronger engine with measured performance improvements and documented design tradeoffs.

---

## Ground Rules

1. Write the code manually.
2. Understand every line before keeping it.
3. Prefer simple, correct implementations before clever ones.
4. Do not skip testing because the code "looks right."
5. Treat debugging as part of learning, not as failure.
6. Keep notes on design changes and lessons learned.

---

## What This Project Should Teach

By the end, this repository should teach:

1. practical C++
2. system decomposition
3. debugging discipline
4. performance reasoning
5. how real engineering skill is built through iteration

---

## How I Want To Use AI On This Project

AI should not write the project for me.

Useful roles for AI:

1. review my code
2. point out bugs or bad assumptions
3. explain compiler errors
4. help me reason about tradeoffs
5. suggest what to learn next

Unwanted role:

1. generating the whole implementation for me

---

## Immediate Starting Point

Restart from a blank codebase.

The first task is to set up Phase 1 from scratch and rebuild the matching engine intentionally, as if the earlier version never existed.
