#pragma once
#include "../common/types.hpp"

struct PriceLevel {
  Price price{0};

  Quantity total_quantity{0};

  Index head_order{INVALID_INDEX};

  Index tail_order{INVALID_INDEX};

  inline bool is_empty() const noexcept { return head_order == INVALID_INDEX; }
};