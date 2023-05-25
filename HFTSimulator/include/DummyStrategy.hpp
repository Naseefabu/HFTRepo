#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "SPSCQueue.hpp"
#include "types.hpp"
#include "utils.hpp"

class DummyStrategy {
public:
    QueueManager& qm;
    DummyStrategy(QueueManager& q) : qm(q) {}

    void run() {
        while (true) {
        	std::cout << "strategy running" << std::endl;
            size_t c = (qm.COINBASE_INPUT_QUEUE.get()).size();
            std::cout << "size" << c <<std::endl;
            place_market_order(true, true, "ETH-USD", 1, generateOrderId(), c);
            std::chrono::seconds sleepDuration(5);
            std::this_thread::sleep_for(sleepDuration);
        }
    }

private:
    int orderIdCounter = 0;

    void place_market_order(bool is_market, bool buy_sell, std::string symbol, int64_t qty, std::string order_id, size_t si) {
        Trader_Order mo(is_market, order_id, symbol, buy_sell, qty, si);
        
        (qm.STRATEGY_ORDER_QUEUE.get()).push(mo);
    }

    std::string generateOrderId() {
        return "Order" + std::to_string(orderIdCounter++);
    }

    void fireOrderEvent(double price, double qty, const std::string& orderId) {
        // Fire the order event with the given price, quantity, and order ID
        std::cout << "Order Event Fired - Price: " << price << ", Quantity: " << qty << ", Order ID: " << orderId << std::endl;
    }

};
