#pragma once
#include <cstdint>

#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

inline uint64_t rdtsc() noexcept { return __rdtsc(); }