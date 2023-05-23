#include <iostream>
#include "SPSCQueue.hpp"
#include "utils.hpp"
#include "CoinbaseWebsocket.hpp"
#include "BitfinexWebsocket.hpp"
#include "CoinbaseOrderbookManager.hpp"
#include "coinbase-api.hpp"
#include "DummyStrategy.hpp"

using namespace rigtorp;


int main(){
    std::cout << "HFTsim" << std::endl;


    ssl::context ctx1{ssl::context::tlsv12_client};
    ctx1.set_verify_mode(ssl::verify_peer);
    ctx1.set_default_verify_paths();

    SPSCQueue<json> COINBASE_INPUT_QUEUE(100000);
    SPSCQueue<json> BITFINEX_INPUT_QUEUE(100000);
    SPSCQueue<Trader_Order> STRATEGY_ORDER_QUEUE(100000);
    SPSCQueue<Trader_fills> STRATEGY_FILLS_QUEUE(100000);


    std::vector<std::string> bitfinex_symbols;
    // Add symbols
    bitfinex_symbols.push_back("tBTCUSD");

    std::vector<std::string> coinbase_symbols;
    // Add symbols
    coinbase_symbols.push_back("ETH-USD");


    std::thread bitfinex_websocket_run([&ctx1,&bitfinex_symbols, &BITFINEX_INPUT_QUEUE]() { 
         set_core_affinity(0);
         run_bitfinex_event_loop(bitfinex_symbols, ctx1, std::ref(BITFINEX_INPUT_QUEUE)); 
    
    });

    std::thread coinbase_websocket_run([&ctx1,&coinbase_symbols, &COINBASE_INPUT_QUEUE]() { 
         set_core_affinity(1);
         run_coinbase_event_loop(coinbase_symbols, ctx1, std::ref(COINBASE_INPUT_QUEUE)); 
    
    });

    std::thread orderbook_manager_run([&ctx1,&coinbase_symbols, &COINBASE_INPUT_QUEUE]() { 
         set_core_affinity(2);
         OrderBookManager orderbook_manager(1000000,true,true, std::ref(COINBASE_INPUT_QUEUE));
         orderbook_manager.run();  
    
    });


    std::thread dummy_strategy_run([&ctx1,&STRATEGY_ORDER_QUEUE, &STRATEGY_FILLS_QUEUE]() { 
         set_core_affinity(3);
         DummyStrategy strategy(std::ref(STRATEGY_ORDER_QUEUE), std::ref(STRATEGY_FILLS_QUEUE));
         strategy.run();  
    
    });


    bitfinex_websocket_run.detach();
    coinbase_websocket_run.detach();
    orderbook_manager_run.detach();
    dummy_strategy_run.detach();


    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1)); 
    }

}