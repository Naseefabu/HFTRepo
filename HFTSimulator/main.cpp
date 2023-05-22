#include <iostream>
#include "SPSCQueue.hpp"
#include "utils.hpp"
#include "CoinbaseWebsocket.hpp"
#include "BitfinexWebsocket.hpp"
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
        set_core_affinity(0);
        run_coinbase_event_loop(coinbase_symbols, ctx1, std::ref(COINBASE_INPUT_QUEUE)); 
    
    });

    bitfinex_websocket_run.detach();
    coinbase_websocket_run.detach();


    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1)); 
    }




}