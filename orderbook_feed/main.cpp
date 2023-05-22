#include "CoinbaseWebsocket.hpp"
#include "CBOrderBookManager.hpp"


void run_event_loop(const std::vector<std::string>& symbols, ssl::context& ctx, SPSCQueue<json>&q)
{
    std::vector<std::shared_ptr<coinbaseWS>> ws_objects; // to make scope outside of for loop
    net::io_context ioc; // one event loop for all symbols
    for (const auto& symbol : symbols) {
        std::cout << symbol << std::endl;
        auto coinbasews = std::make_shared<coinbaseWS>(ioc.get_executor(), ctx, q);  
        ws_objects.push_back(coinbasews);
        coinbasews->subscribe_orderbook(symbol);
    }

    ioc.run(); // this will block until all asynchronous operations have completed
}

void set_core_affinity(unsigned int core_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}


int main(){

    ssl::context ctx1{ssl::context::tlsv12_client};
    ctx1.set_verify_mode(ssl::verify_peer);
    ctx1.set_default_verify_paths();

    std::vector<std::string> symbols;
    
    // Add symbols
    symbols.push_back("ETH-USD");
    symbols.push_back("ETH-EUR");

    SPSCQueue<json> INPUT_QUEUE(100000);

    std::thread coinbase_websocket_run([&ctx1,&symbols, &INPUT_QUEUE]() { 
        set_core_affinity(0);
        run_event_loop(symbols, ctx1, std::ref(INPUT_QUEUE)); 
    
    });

    std::thread coinbase_bookbuilder_run([&INPUT_QUEUE]() { 
        set_core_affinity(1);
        CoinbaseOrderBookManager orderbook_manager(1000000,true,true, std::ref(INPUT_QUEUE));
        orderbook_manager.run();    
    });


    coinbase_websocket_run.detach();
    coinbase_bookbuilder_run.detach();

    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1)); 
    }

}