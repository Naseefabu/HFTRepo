#pragma once 

struct Trader_Order {
    size_t coinbase_exchange_size;
    bool is_market;
    std::string orderid;
    std::string symbol;
    bool buy_sell;
    int64_t qty;

    Trader_Order(bool is_marke, std::string orderi, std::string symbo, bool buy_sel, int64_t size, size_t cb_size)
        : is_market(is_marke), orderid(orderi), symbol(symbo), buy_sell(buy_sel), qty(size), coinbase_exchange_size(cb_size){}
};

struct Trader_fills{
	std::string symbol;
	double qty;
    Trader_fills(std::string symb, double sizew){
    	symbol = symb;
    	qty = sizew;
    }
};