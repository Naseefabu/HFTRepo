#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "SPSCQueue.hpp"

struct Trader_Order {
    bool is_market;
    std::string orderid;
    std::string symbol;
    bool buy_sell;
    int64_t qty;

    Trader_Order(bool is_marke, std::string orderi, std::string symbo, bool buy_sel, int64_t size)
        : is_market(is_marke), orderid(orderi), symbol(symbo), buy_sell(buy_sel), qty(size) {}
};

struct Trader_fills{
	std::string symbol;
	double qty;
    Trader_fills(std::string symb, double sizew){
    	symbol = symb;
    	qty = sizew;
    }
};

class DummyStrategy {
public:
    DummyStrategy(SPSCQueue<Trader_Order>& ORDERS, SPSCQueue<Trader_fills>& FILLS)
        : ORDERS_QUEUE(ORDERS), FILLS_QUEUE(FILLS) {}

    void run() {
        while (true) {
        	std::cout << "strategy running" << std::endl;
            place_market_order(true, true, "ETH-USD", 1, generateOrderId());
            std::chrono::seconds sleepDuration(5);
            std::this_thread::sleep_for(sleepDuration);

        }
    }

private:
    int orderIdCounter = 0;

    void place_market_order(bool is_market, bool buy_sell, std::string symbol, int64_t qty, std::string order_id) {
        Trader_Order mo(is_market, order_id, symbol, buy_sell, qty);
        (ORDERS_QUEUE.get()).push(mo);
    }

    std::string generateOrderId() {
        return "Order" + std::to_string(orderIdCounter++);
    }

    void fireOrderEvent(double price, double qty, const std::string& orderId) {
        // Fire the order event with the given price, quantity, and order ID
        std::cout << "Order Event Fired - Price: " << price << ", Quantity: " << qty << ", Order ID: " << orderId << std::endl;
    }

    std::reference_wrapper<SPSCQueue<Trader_Order>> ORDERS_QUEUE;
    std::reference_wrapper<SPSCQueue<Trader_fills>> FILLS_QUEUE;
};
