#pragma once
#include <string>

struct OrderbookMessage{
    std::string symbol;
    double price;
    double quantity;
    std::string type;
};