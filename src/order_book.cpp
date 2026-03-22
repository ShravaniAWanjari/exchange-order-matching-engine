#include "order_book.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

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

    if (orderIndex_.find(request.id) != orderIndex_.end()){
        std::cout << "Rejected Order : duplicate id\n";
        return;
    }

    Order incoming = makeOrder(request);

    if (incoming.side== Side::Buy){
        while(incoming.quantity >0 && !asks_.empty()){
            auto bestAskIt = asks_.begin();

            if (bestAskIt->first > incoming.price){
                break;
            }

            PriceLevel& askLevel = bestAskIt->second; //kinda confused about this part -> first -> second

            while (incoming.quantity > 0 && !askLevel.orders.empty()) {
                Order& restingAsk = askLevel.orders.front(); //what's restingAsk now

                std::uint32_t tradedQuantity = std::min(incoming.quantity, restingAsk.quantity);

                std::cout << "Trade: "
                          << tradedQuantity
                          << " @ "
                          << restingAsk.price
                          << '\n';
      
                
                incoming.quantity -= tradedQuantity;
                restingAsk.quantity -= tradedQuantity;
                askLevel.totalQuantity -= tradedQuantity;


                if (restingAsk.quantity == 0){
                    orderIndex_.erase(restingAsk.id);
                    askLevel.orders.pop_front();                
                }            
            }

            if (askLevel.orders.empty()){
                asks_.erase(bestAskIt);
            }
        }

        if (incoming.quantity > 0){
            PriceLevel& bidLevel = bids_[incoming.price];
            bidLevel.orders.push_back(std::move(incoming));

            Order& storedOrder = bidLevel.orders.back();
            bidLevel.totalQuantity += storedOrder.quantity;
            orderIndex_[storedOrder.id] = {storedOrder.side, storedOrder.price};
        }
    } else {
        while (incoming.quantity > 0 && !bids_.empty()) {
            auto bestBidIt = bids_.begin();

            if (bestBidIt->first < incoming.price) {
                break;
            }

            PriceLevel& bidLevel = bestBidIt->second;

            while (incoming.quantity > 0 && !bidLevel.orders.empty()) {
                Order& restingBid = bidLevel.orders.front();

                std::uint32_t tradedQuantity =
                    std::min(incoming.quantity, restingBid.quantity);

                    std::cout << "Trade: " 
                              << tradedQuantity
                              << " @ "
                              << restingBid.price
                              << '\n';
          
          

                incoming.quantity -= tradedQuantity;
                restingBid.quantity -= tradedQuantity;
                bidLevel.totalQuantity -= tradedQuantity;

                if (restingBid.quantity == 0) {
                    orderIndex_.erase(restingBid.id);
                    bidLevel.orders.pop_front();
                }
            }

            if (bidLevel.orders.empty()) {
                bids_.erase(bestBidIt);
            }
        }

        if (incoming.quantity > 0) {
            PriceLevel& askLevel = asks_[incoming.price];
            askLevel.orders.push_back(std::move(incoming));

            Order& storedOrder = askLevel.orders.back();
            askLevel.totalQuantity += storedOrder.quantity;
            orderIndex_[storedOrder.id] = {storedOrder.side, storedOrder.price};
        }
    }
}

bool OrderBook::cancelOrder(std::uint64_t orderId) {
    auto indexIt = orderIndex_.find(orderId);

    if (indexIt == orderIndex_.end()) {
        return false;
    }

    OrderLocator locator = indexIt->second;

    if (locator.side == Side::Buy) {
        auto levelIt = bids_.find(locator.price);

        if (levelIt == bids_.end()) {
            return false;
        }

        PriceLevel& bidLevel = levelIt->second;

        for (auto orderIt = bidLevel.orders.begin();
             orderIt != bidLevel.orders.end();
             ++orderIt) {
            if (orderIt->id == orderId) {
                bidLevel.totalQuantity -= orderIt->quantity;
                bidLevel.orders.erase(orderIt);
                orderIndex_.erase(indexIt);

                if (bidLevel.orders.empty()) {
                    bids_.erase(levelIt);
                }

                return true;
            }
        }
    } else {
        auto levelIt = asks_.find(locator.price);

        if (levelIt == asks_.end()) {
            return false;
        }

        PriceLevel& askLevel = levelIt->second;

        for (auto orderIt = askLevel.orders.begin();
             orderIt != askLevel.orders.end();
             ++orderIt) {
            if (orderIt->id == orderId) {
                askLevel.totalQuantity -= orderIt->quantity;
                askLevel.orders.erase(orderIt);
                orderIndex_.erase(indexIt);

                if (askLevel.orders.empty()) {
                    asks_.erase(levelIt);
                }

                return true;
            }
        }
    }

    return false;
}


void OrderBook::printBook() const {
    std::cout << "BIDS\n";
    for (const auto& level : bids_) {
        std::cout << level.first
                  << " : "
                  << level.second.totalQuantity
                  << '\n';
    }

    std::cout << "\nASKS\n";
    for (const auto& level : asks_) {
        std::cout << level.first
                  << " : "
                  << level.second.totalQuantity
                  << '\n';
    }
}
