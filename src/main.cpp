#include "common/id_generator.hpp"
#include "common/order.hpp"
#include <cstddef>
#include <iostream>

int main() {
  std::cout << "---Memory Layout Verification ---\n";

  std::cout << "Size of Order: " << sizeof(lob::Order) << " bytes\n";
  std::cout << "Alignment of Order: " << alignof(lob::Order) << "bytes\n";

  std::cout << "\n--- Memory Offsets ---\n";

  std::cout << "Offset of id: " << offsetof(lob::Order, id) << "\n";
  std::cout << ""
}