#include <iostream>
#include "SPSCQueue.hpp"
#include "utils.hpp"
#include "CoinbaseWebsocket.hpp"
#include "BitfinexWebsocket.hpp"
#include "CoinbaseOrderbookManager.hpp"
#include "coinbase-api.hpp"

using namespace rigtorp;


int main(){
    std::cout << "HFTsim" << std::endl;


    ssl::context ctx1{ssl::context::tlsv12_client};
    ctx1.set_verify_mode(ssl::verify_peer);
    ctx1.set_default_verify_paths();

    SPSCQueue<json> COINBASE_INPUT_QUEUE(100000);
    SPSCQueue<json> BITFINEX_INPUT_QUEUE(100000);
    SPSCQueue<json> STRATEGY_ORDER_QUEUE(100000);
    SPSCQueue<json> STRATEGY_FILLS_QUEUE(100000);


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

    std::thread coinbase_orderbook_manager_run([&ctx1,&coinbase_symbols, &COINBASE_INPUT_QUEUE]() { 
         set_core_affinity(2);
         CoinbaseOrderBookManager orderbook_manager(1000000,true,true, std::ref(COINBASE_INPUT_QUEUE));
         orderbook_manager.run();  
    
    });

    bitfinex_websocket_run.detach();
    coinbase_websocket_run.detach();
    coinbase_orderbook_manager_run.detach();

    net::io_context ioc;

    auto coinbaseapi = std::make_shared<coinbaseAPI>(ioc.get_executor(),ctx1,ioc);
    json response = coinbaseapi->get_snapshot("ETH-USD",3);

    for (const auto& bid : response["bids"]) {
        std::cout << "buys" <<  std::endl;
        std::cout << "id : " << bid[2] << std::endl;
        std::cout << "price : " << bid[0] << std::endl;
        std::cout << "size : " << bid[1] << std::endl;
    }

    for (const auto& ask : response["asks"]) {
        std::cout << "sells" << std::endl;
        std::cout << "id : " << ask[2] << std::endl;
        std::cout << "price : " << ask[0] << std::endl;
        std::cout << "size : " << ask[1] << std::endl;
    }



    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1)); 
    }




}