#pragma once

#define BITFINEX_HANDLER(c) beast::bind_front_handler(&bitfinexWS::c, this->shared_from_this())

class bitfinexWS : public std::enable_shared_from_this<bitfinexWS>
{
public:    
    tcp::resolver resolver_;
    Stream ws_;
    beast::flat_buffer buffer_;
    std::string message_text_;
    char const* host = "api-pub.bitfinex.com";
    std::string symb;
    std::string wsTarget_ = "/ws/2";
    std::string host_;
    std::function<void()> message_handler;  
    std::reference_wrapper<SPSCQueue<json>> queue;


  public:

    bitfinexWS(net::any_io_executor ex, ssl::context& ctx, SPSCQueue<json>& qu)
        : resolver_(ex)
        , ws_(ex, ctx)
        , queue(qu)
        {
        }

    void run(json message) {
        if (!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), host)) {
            throw boost::system::system_error(
                error_code(::ERR_get_error(), net::error::get_ssl_category()));
        }
        host_ = host;
        message_text_ = message.dump();

        resolver_.async_resolve(host_, "443", BITFINEX_HANDLER(on_resolve));
    }

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
        if (ec)
            return fail_ws(ec, "resolve");

        if (!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), host_.c_str())) {
            throw beast::system_error{
                error_code(::ERR_get_error(), net::error::get_ssl_category())};
        }

        get_lowest_layer(ws_).expires_after(30s);

        beast::get_lowest_layer(ws_).async_connect(results, BITFINEX_HANDLER(on_connect));
    }

    void on_connect(beast::error_code                                           ec,
                    [[maybe_unused]] tcp::resolver::results_type::endpoint_type ep) {
        if (ec)
            return fail_ws(ec, "connect");

        // Perform the SSL handshake
        ws_.next_layer().async_handshake(ssl::stream_base::client, BITFINEX_HANDLER(on_ssl_handshake));
    }

    void on_ssl_handshake(beast::error_code ec) {
        if (ec)
            return fail_ws(ec, "ssl_handshake");

        beast::get_lowest_layer(ws_).expires_never();

        ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

        ws_.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
            req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
        }));

        std::cout << "using host_: " << host_ << std::endl;
        ws_.async_handshake(host_, wsTarget_, BITFINEX_HANDLER(on_handshake));
    }

    void on_handshake(beast::error_code ec) {
        if (ec) {
            return fail_ws(ec, "handshake");
        }

        std::cout << "Sending : " << message_text_ << std::endl;

        ws_.async_write(net::buffer(message_text_), BITFINEX_HANDLER(on_write));
    }

    void on_write(beast::error_code ec, size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail_ws(ec, "write");

        ws_.async_read(buffer_, BITFINEX_HANDLER(on_message));
    }

    void on_message(beast::error_code ec, size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec)
            return fail_ws(ec, "read");

        buffer_.clear();
        ws_.async_read(buffer_, [this](beast::error_code ec, size_t n) {
            if (ec)
                return fail_ws(ec, "read");
            message_handler();
            buffer_.clear();
            ws_.async_read(buffer_, BITFINEX_HANDLER(on_message));
        });
    }

    void on_close(beast::error_code ec) {
        if (ec)
            return fail_ws(ec, "close");

        std::cout << beast::make_printable(buffer_.data()) << std::endl;
    }

  
  void subscribe_orderbook(const std::string& market) // full channel
  {
    symb = market;

    json payload = {{"event", "subscribe"},
                {"channel", "book"},
                {"prec","R0"},
                {"symbol", market}};

    message_handler = [this]() {
        
        json payload = json::parse(beast::buffers_to_string(buffer_.cdata()));
        payload["symbol"] = symb;
        (queue.get()).push(payload);
        std::cout << "bitfinex payload" << payload <<std::endl;
    };

    run(payload);            
  }


};


void run_bitfinex_event_loop(const std::vector<std::string>& symbols, ssl::context& ctx, SPSCQueue<json>&q)
{
    std::vector<std::shared_ptr<bitfinexWS>> ws_objects; // to make scope outside of for loop
    net::io_context ioc; // one event loop for all symbols
    for (const auto& symbol : symbols) {
        std::cout << symbol << std::endl;
        auto bitfinexws = std::make_shared<bitfinexWS>(ioc.get_executor(), ctx, q);  
        ws_objects.push_back(bitfinexws);
        bitfinexws->subscribe_orderbook(symbol);
    }

    ioc.run(); // this will block until all asynchronous operations have completed
}
