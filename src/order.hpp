#ifndef ORDER_HPP
#define ORDER_HPP

#include <cstdint>

enum class Side{
    Buy,
    Sell
};

struct OrderRequest{
    std::uint64_t id;
    double price;
    std::uint32_t quantity;
    Side side;

    OrderRequest(
        std::uint64_t orderId,
        double orderPrice.
        std::uint32_t quantity,
        Side orderSide):
        id(orderId),
        price(orderPrice),
        quantity(orderQuantity),
        side(orderSide){}
}

struct Order{
    std::uint64_t id;
    double price;
    std::uint64_t quantity;
    Side side;
    std::uint64_t sequence;

    Order(std::uint64_t orderId,
    double orderPrice,
    std::uint32_t orderQuantity,
    Side orderSide,
    std::uint64_t orderSequence) :
    id(orderId),
    price(orderPrice),
    quantity(orderQuantity),
    side(orderSide),
    sequence(orderSequence) {}
};

#endif