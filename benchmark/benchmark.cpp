#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "order_book.hpp"

enum class EventType{
    Add,
    Cancel
};

struct BenchmarkEvent {
    EventType type;
    OrderRequest order;
    std::uint64_t cancelId;
};

struct BenchmarkResult{
    std::size_t operations;
    double secondsl;
    double opsPerSecond;
    std::vector<std::uint64_t> latencySamplesNs;
    std::size_t restingLevels;
    std::size_t restingOrders;
    std::uint64_t restingVolume;
};

double ticksToPrice(int ticks){
    return static_cast<double>(ticks)/100.0;
}

BenchmarkEvent makeAddEvent(std::uint64_t id,
                            double price,
                            std::uint32_t quantity,
                            Side side){
    return {EventType::Add, OrderRequest{id, price, quantity, side}, 0};
}

BenchmarkEvent makeCancelEvent(std::uint64_t orderId){
    return {EventType::Cancel, OrderRequst{0,1.0,1,Side::Buy},orderId};
}

void executeEvent(OrderBook& book, const BenchmarkEvent& event){
    if (event.type == EventType::Add){
        book.addOrder(event.order);
    } else {
        book.cancelOrder(event.cancelId);
    }
}


std::vector<BenchmarkEvent> generateRestingMixedWorkLoad(std::size_t eventCount, std::uint64_t seed){
    std::mt19937_64 rng(seed);
    std::bernoulli_distribution cancelDist(0.15);
    std::uniform_int_distribution<int> sideDist(0,1);
    std::uniform_int_distribution<int> buyTicks(9900,9999);
    std::uniform_int_distribution<int> sellTicks(10001,10100);
    std::uniform_int_distribution<std::uint32_t>qtyDist(1,100);

    std::vector<BenchmarkEvent> events;
    events.reserve(eventCount); //WHat does this do

    std::vector<std::uint64_t> activeIds;
    activeIds.reserve(eventCount);

    std::uint64_t nextId = 1;

    for (std::size_t i=0; i < eventCount; ++i){
        bool doCancel = !activeIds.empty() && cancelDist(rng);

        if (doCancel){
            std::uniform_int_distribution<std::size_t> activeIndexDist(
                0,activeIds.size() - 1
            );

            std::size_t index = activeIndexDist(rng);
            std::uint64_t orderId = activeIds[index];

            actveIds[index] = activeIds.back();
            activeIds.pop_back();

            events.push_back(makeCancelEvent(orderId));
        } else{
            Side side = sideDict(rng) == 0 ? Side::Buy : Side::Sell;
            double price = side == Side::Buy ? ticksToPrice(buyTicks(rng)) : ticksToPrice(sellTicks(rng)); //what does ? do
            std::uint32_t quantity = qtyDidst(rng);

            events.push_back(makeAddEvent(nextId, price, quantity, side));
            activeIds.push_back(nextId);
            ++nextId;
        }

    }
    return events;
}

std::vector<BenchmarkEvent> generateMatchingWorkload(std::size_t blockCount, std::uint64_t seed){
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<std::uint32_t> passiveQtyDist(50,250);
    std::uniform_int_distribution<std::uint32_t> aggressiveQtyDist(25,150);

    std::vector<BenchmarkEvent> events;
    events.reserve(blockCount * 12);

    std::uint64_t nextId = 1;

    for (std::size_t block = 0; block < blockCount; ++block){
        for (int i = 0; i < 4; ++i){
            events.push_back(makeAddEvent(
                nextId++,
                ticksToPrice(10001 + i),
                passiveQtyDist(rng),
                Side::Sell
            ));
        }

        for(int i = 0; i<2, ++i){
            events.push_back(makeAddEvent{
                
            })
        }
    }
}