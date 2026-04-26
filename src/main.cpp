#include <iostream>

#include "order.hpp"
#include "order_book.hpp"

void printScenarioTitle(const char* title) {
    std::cout << "\n=== " << title << " ===\n";
}

// Each scenario gets a fresh book so results stay isolated.
void runPartialFillAcrossLevels() {
    printScenarioTitle("Scenario 1: Partial Fill Across Levels");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 50, Side::Sell});
    book.addOrder(OrderRequest{2, 9900, 40, Side::Sell});
    book.addOrder(OrderRequest{3, 10100, 60, Side::Buy});

    book.printBook();
}

void runNonCrossingOrders() {
    printScenarioTitle("Scenario 2: Non-Crossing Orders");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 50, Side::Buy});
    book.addOrder(OrderRequest{2, 10200, 40, Side::Sell});

    book.printBook();
}

void runExactFullFill() {
    printScenarioTitle("Scenario 3: Exact Full Fill");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 50, Side::Sell});
    book.addOrder(OrderRequest{2, 10000, 50, Side::Buy});

    book.printBook();
}

void runFifoAtSamePrice() {
    printScenarioTitle("Scenario 4: FIFO At Same Price");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 20, Side::Sell});
    book.addOrder(OrderRequest{2, 10000, 40, Side::Sell});
    book.addOrder(OrderRequest{3, 10000, 30, Side::Buy});

    book.printBook();
}

void runInvalidOrderRejection() {
    printScenarioTitle("Scenario 5: Invalid Order Rejection");

    OrderBook book;
    book.addOrder(OrderRequest{1, 0, 50, Side::Buy});
    book.addOrder(OrderRequest{2, 10000, 0, Side::Sell});
    book.addOrder(OrderRequest{3, 10100, 25, Side::Buy});

    book.printBook();
}

void runCancelExistingBid() {
    printScenarioTitle("Scenario 6: Cancel Existing Bid");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 20, Side::Buy});
    book.addOrder(OrderRequest{2, 10000, 40, Side::Buy});
    book.addOrder(OrderRequest{3, 10100, 10, Side::Buy});

    bool cancelled = book.cancelOrder(2);
    std::cout << "Cancel 2: " << (cancelled ? "success" : "failed") << '\n';

    book.printBook();
}

void runCancelExistingAsk() {
    printScenarioTitle("Scenario 7: Cancel Existing Ask");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10200, 20, Side::Sell});
    book.addOrder(OrderRequest{2, 10300, 40, Side::Sell});

    bool cancelled = book.cancelOrder(1);
    std::cout << "Cancel 1: " << (cancelled ? "success" : "failed") << '\n';

    book.printBook();
}

void runCancelMissingOrder() {
    printScenarioTitle("Scenario 8: Cancel Missing Order");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 20, Side::Buy});

    bool cancelled = book.cancelOrder(99);
    std::cout << "Cancel 99: " << (cancelled ? "success" : "failed") << '\n';

    book.printBook();
}

void runDuplicateActiveIdRejection() {
    printScenarioTitle("Scenario 9: Duplicate Active Id Rejection");

    OrderBook book;
    book.addOrder(OrderRequest{1, 10000, 20, Side::Buy});
    book.addOrder(OrderRequest{1, 10100, 30, Side::Sell});

    book.printBook();
}

int main() {
    runPartialFillAcrossLevels();
    runNonCrossingOrders();
    runExactFullFill();
    runFifoAtSamePrice();
    runInvalidOrderRejection();
    runCancelExistingBid();
    runCancelExistingAsk();
    runCancelMissingOrder();
    runDuplicateActiveIdRejection();

    return 0;
}
