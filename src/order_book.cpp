#include "order_book.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>

namespace {
    std::uint32_t totalQuantityAtLevel(const OrderBook::OrderQueue& orders){
        std::uint32_t total = 0;

        for (const Order& order : orders){
            total += order.quantity;
        }

        return total;
    }
}

OrderBook::OrderBook() : nextSequence_(1) {}

Order OrderBook::makeOrder(const OrderRequest& request) {
    return Order(
        request.id,
        request.price,
        request.quantity,
        request.side,
        nextSequence_++
    );
}


void OrderBook::addOrder(const OrderRequest& request){
    if (request.price <= 0.0 || request.quantity ==0){
        std::cout << "Rejected order: invalid price or quantity\n";
        return;
    }

    Order incoming = makeOrder(request);

    if (incoming.side== Side::Buy){
        while(incoming.quantity >0 && !asks_.empty()){
            auto bestAskIt = asks_.begin();

            if (bestAskIt->first > incoming.price){
                break;
            }

            OrderQueue& askQueue = bestAskIt->second; //kinda confused about this part -> first -> second

            while (incoming.quantity > 0 && !askQueue.empty()) {
                Order& restingAsk = askQueue.front(); //what's restingAsk now

                std::uint32_t tradedQuantity = std::min(incoming.quantity, restingAsk.quantity);

                std::cout << "Trade : "
                          << tradedQuantity
                          << "@"
                          << restingAsk.price
                          << "\n" ;
                
                incoming.quantity -= tradedQuantity;
                restingAsk.quantity -= tradedQuantity;

                if (restingAsk.quantity == 0){
                    askQueue.pop_front();                
                }            
            }

            if (askQueue.empty()){
                asks_.erase(bestAskIt);
            }
        }

        if (incoming.quantity > 0){
            bids_[incoming.price].push_back(std::move(incoming));
        }
    } else {
        while (incoming.quantity > 0 && !bids_.empty()) {
            auto bestBidIt = bids_.begin();

            if (bestBidIt->first < incoming.price) {
                break;
            }

            OrderQueue& bidQueue = bestBidIt->second;

            while (incoming.quantity > 0 && !bidQueue.empty()) {
                Order& restingBid = bidQueue.front();

                std::uint32_t tradedQuantity =
                    std::min(incoming.quantity, restingBid.quantity);

                std::cout << "Trade: "
                          << tradedQuantity
                          << " @ "
                          << restingBid.price
                          << '\n';

                incoming.quantity -= tradedQuantity;
                restingBid.quantity -= tradedQuantity;

                if (restingBid.quantity == 0) {
                    bidQueue.pop_front();
                }
            }

            if (bidQueue.empty()) {
                bids_.erase(bestBidIt);
            }
        }

        if (incoming.quantity > 0) {
            asks_[incoming.price].push_back(std::move(incoming));
        }
    }
}


void OrderBook::printBook() const {
    std::cout << "BIDS\n";
    for (const auto& level : bids_) {
        std::cout << level.first
                  << " : "
                  << totalQuantityAtLevel(level.second)
                  << '\n';
    }

    std::cout << "\nASKS\n";
    for (const auto& level : asks_) {
        std::cout << level.first
                  << " : "
                  << totalQuantityAtLevel(level.second)
                  << '\n';
    }
}
