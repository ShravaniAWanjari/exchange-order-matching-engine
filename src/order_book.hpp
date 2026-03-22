#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <unordered_map>

#include "order.hpp"

struct PriceLevel{
    std::deque<Order> orders;
    std::uint32_t totalQuantity;

    PriceLevel() : totalQuantity(0){};
};

struct OrderLocator {
    Side side;
    double price;
};

class OrderBook{
    public:
    using AskLevels = std::map<double, PriceLevel>;
    using BidLevels = std::map<double, PriceLevel, std::greater<double>>;

    OrderBook();

    void addOrder(const OrderRequest& request);
    bool cancelOrder(std::uint64_t orderId);
    void printBook() const;

private:
    BidLevels bids_;
    AskLevels asks_;
    std::unordered_map<std::uint64_t, OrderLocator> orderIndex_;
    std::uint64_t nextSequence_;

    Order makeOrder(const OrderRequest& request);
};

#endif