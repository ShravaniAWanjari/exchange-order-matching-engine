#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

#include "order.hpp"

struct OrderSnapshot {
    std::uint64_t id;
    std::uint32_t quantity;
    std::uint64_t sequence;
};

struct LevelSnapshot {
    std::int64_t price;
    std::uint32_t totalQuantity;
    std::vector<OrderSnapshot> orders;
};

struct BookSnapshot {
    std::vector<LevelSnapshot> bids;
    std::vector<LevelSnapshot> asks;
};

struct PriceLevel{
    std::list<Order> orders;
    std::uint32_t totalQuantity;

    PriceLevel() : totalQuantity(0){};
};

struct OrderLocator {
    Side side;
    std::int64_t price;
    std::list<Order>::iterator iterator;
};

class OrderBook{
    public:
    using AskLevels = std::map<std::int64_t, PriceLevel>;
    using BidLevels = std::map<std::int64_t, PriceLevel, std::greater<std::int64_t>>;

    explicit OrderBook(bool enableLogging = true);

    void addOrder(const OrderRequest& request);
    bool cancelOrder(std::uint64_t orderId);

    BookSnapshot snapshot() const;
    void printBook() const;
    void printDetailedBook() const;

private:
    BidLevels bids_;
    AskLevels asks_;
    std::unordered_map<std::uint64_t, OrderLocator> orderIndex_;
    std::uint64_t nextSequence_;
    bool loggingEnabled_;

    Order makeOrder(const OrderRequest& request);
};

#endif