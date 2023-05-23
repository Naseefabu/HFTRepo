#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <nlohmann/json.hpp>
#include<string>
#include <iostream>
#include <string>
#include <chrono>
#include <exception>
#include "utils.hpp"

namespace beast = boost::beast;         
namespace http  = beast::http;          
namespace net   = boost::asio;          
namespace ssl   = boost::asio::ssl;     
using tcp       = boost::asio::ip::tcp; 

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
using executor = net::any_io_executor; 

class coinbaseAPI : public std::enable_shared_from_this<coinbaseAPI>
{
private:
    std::string base_api = "https://api.exchange.coinbase.com/";
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    http::response<http::string_body> res_;
    std::string api_key = "db11f28ba4fa14b548ee568c4d236745";
    std::string passphrase = "f717xa2bkqi";
    net::io_context& ioc;
    ssl::context& ctx;
    std::string sign;

public:
    coinbaseAPI(executor ex, ssl::context& ctxe, net::io_context &ioce)
        : resolver_(ex), stream_(ex, ctxe), ioc(ioce), ctx(ctxe)
    {
    }

    http::response<http::string_body> http_call(const std::string& url, http::verb action)
    {
        std::string const host = get_host_from_url(url);
        std::string const target = get_target_from_url(url);

        std::string const service = "https";

        SSL_set_tlsext_host_name(stream_.native_handle(), host.c_str());

        req_.method(action);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req_.set("CB-ACCESS-KEY", api_key);
        req_.set("CB-ACCESS-PASSPHRASE", passphrase);
        req_.set("Content-Type", "application/json");
        req_.prepare_payload();

        auto const results = resolver_.resolve(host, service);
        beast::get_lowest_layer(stream_).connect(results);

        stream_.handshake(ssl::stream_base::client);

        http::write(stream_, req_);
        http::read(stream_, buffer_, res_);
        beast::error_code ec;
        stream_.shutdown(ec);

        return res_;
    }

    json get_snapshot(std::string market, int level)
    {
        std::string method = "products/" + market + "/book";
        method += "?level=" + std::to_string(level);
        std::string url = base_api + method;
        return json::parse(http_call(url, http::verb::get).body());
    }

private:
    std::string get_host_from_url(const std::string& url)
    {
        size_t start = url.find("://");
        if (start == std::string::npos)
            throw std::invalid_argument("Invalid URL");

        start += 3;
        size_t end = url.find('/', start);
        if (end == std::string::npos)
            throw std::invalid_argument("Invalid URL");

        return url.substr(start, end - start);
    }

    std::string get_target_from_url(const std::string& url)
    {
        size_t start = url.find('/', url.find("://") + 3);
        if (start == std::string::npos)
            return "/";
        return url.substr(start);
    }
};
