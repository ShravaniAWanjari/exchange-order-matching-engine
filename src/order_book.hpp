#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <cstdint>
#include <deque>
#include <functional>
#include <map>

#include "order.hpp"

class OrderBook{
    public:
    using OrderQueue = std::deque<Order>;
    using AskLevels = std::map<double, OrderQueue>;
    using BidLevels = std::map<double, OrderQueue, std::greater<double>>;

    OrderBook();

    void addOrder(const OrderRequest& request);
    void printBook() const;

private:
    BidLevels bids_;
    AskLevels asks_;
    std::uint64_t nextSequence_;

    Order makeOrder(const OrderRequest& request);
};

#endif