#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

class LatencyHistogram {
public:
  static constexpr size_t NUM_BUCKETS = 64;
  static constexpr uint64_t CYCLES_PER_BUCKET = 32;
  inline void record_latency(uint64_t cycles) noexcept {
    size_t bucket = cycles / CYCLES_PER_BUCKET;

    if (bucket >= NUM_BUCKETS) [[unlikely]] {
      bucket = NUM_BUCKETS - 1;
    }
    buckets_[bucket].fetch_add(1, std::memory_order_relaxed);
  }

  void take_snapshot(std::array<uint64_t, NUM_BUCKETS> &output) noexcept {
    for (size_t i = 0; i < NUM_BUCKETS; ++i) {
      output[i] = buckets_[i].exchange(0, std::memory_order_relaxed);
    }
  }

private:
  std::array<std::atomic<uint64_t>, NUM_BUCKETS> buckets_{};
};