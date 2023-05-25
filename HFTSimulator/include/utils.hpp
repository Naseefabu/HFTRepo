#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <system_error>
#include <unordered_map>
#include <pthread.h>
#include <sched.h>
#include "SPSCQueue.hpp"
#include <initializer_list>
#include "types.hpp"



namespace beast     = boost::beast;    
namespace http      = beast::http;     
namespace websocket = beast::websocket; 
namespace net       = boost::asio;      
namespace ssl       = net::ssl;
using namespace rigtorp;
using beast::error_code;

using tcp           = net::ip::tcp; 
using json = nlohmann::json;

using TCPStream = beast::tcp_stream;
using SSLStream = beast::ssl_stream<TCPStream>;
using Stream    = websocket::stream<SSLStream>;

using namespace std::chrono_literals;

void fail_ws(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}



void set_core_affinity(unsigned int core_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}



void fail_http(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}


class QueueManager {
public:

    std::reference_wrapper<SPSCQueue<json>> COINBASE_INPUT_QUEUE;
    std::reference_wrapper<SPSCQueue<json>> BITFINEX_INPUT_QUEUE;
    std::reference_wrapper<SPSCQueue<Trader_Order>> STRATEGY_ORDER_QUEUE; 
    std::reference_wrapper<SPSCQueue<Trader_fills>> STRATEGY_FILLS_QUEUE;

    QueueManager(SPSCQueue<json>& coinbase_input_queue, SPSCQueue<json>& bitfinex_input_queue, SPSCQueue<Trader_Order>& strategy_order_queue, SPSCQueue<Trader_fills>& trader_fills_queue) 
                    :COINBASE_INPUT_QUEUE(coinbase_input_queue), BITFINEX_INPUT_QUEUE(bitfinex_input_queue), STRATEGY_FILLS_QUEUE(trader_fills_queue), STRATEGY_ORDER_QUEUE(strategy_order_queue){}

  QueueManager(const QueueManager&) = delete;
  QueueManager &operator=(const QueueManager &) = delete;
};



