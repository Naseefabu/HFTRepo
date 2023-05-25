

#pragma once

#include "HashMap.hpp"
#include <boost/container/flat_map.hpp>
#include <tsl/ordered_map.h>
#include "coinbase-api.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstdlib> 

struct BestPrice {
  int64_t bidqty;
  int64_t bid;
  int64_t ask;
  int64_t askqty;

  BestPrice(int64_t bidqty, int64_t bid, int64_t ask, int64_t askqty)
      : bidqty(bidqty), bid(bid), ask(ask), askqty(askqty) {}

  BestPrice() : bidqty(0), bid(0), ask(0), askqty(0) {}

  friend std::ostream &operator<<(std::ostream &out, const BestPrice &best) {
    out << "BestPrice(" << best.bidqty << ", " << best.bid << ", " << best.ask
        << ", " << best.askqty << ")";
    return out;
  }
};

static constexpr int16_t NOBOOK = std::numeric_limits<int16_t>::max();
struct Order {
	double price = 0;
	double qty = 0;
	bool buy_sell = 0;
	int16_t bookid = NOBOOK;
// what 
	Order(double price, double qty, int16_t buy_sell, int16_t bookid)
	    : price(price), qty(qty), buy_sell(buy_sell), bookid(bookid) {}
	Order() {}
};

class OrderBook {
public:
  struct Level {
    double price = 0;
    double qty = 0;
    uint64_t seqno = 0;
    tsl::ordered_map<std::string, Order> orders;
    //std::multimap<std::string, Order> orders;

    friend std::ostream &operator<<(std::ostream &out, const Level &level) {
      out << "Level(" << level.price << ", " << level.qty << ", " << level.seqno
          << ")";
      return out;
    }
  };
  uint64_t seq=0;

public:
  OrderBook(void *data = NULL) : data_(NULL) {}

  BestPrice GetBestPrice() const {
    auto buy = buy_.rbegin();
    auto sell = sell_.rbegin();
    BestPrice bp;
    if (buy != buy_.rend()) {
      bp.bidqty = buy->second.qty;
      bp.bid = buy->second.price;
    }
    if (sell != sell_.rend()) {
      bp.askqty = sell->second.qty;
      bp.ask = sell->second.price;
    }
    return bp;
  }

  void *GetUserData() const { return data_; }

  void SetUserData(void *data) { data_ = data; }

  bool Add(uint64_t seqno, std::string ref, bool buy_sell, double price, double qty, Order order) {
    if (qty <= 0) {
      return false;
    }
    auto &side = buy_sell ? buy_ : sell_;
    int64_t prio = buy_sell ? -price : price;
    
    auto it = side.find(prio);
    if (it == side.end()) {
      Level level;
  		level.price = price;
  		level.seqno = seqno;
  		level.qty = order.qty; // check
  		level.orders.insert({ref,order}); 
  		side.insert(std::make_pair(prio, level));
  		seq = seqno;
  	}
    else{
    	it->second.orders.insert({ref,order}); 
    	it->second.qty += qty;
    	it->second.seqno = seqno;
    	seq = seqno;

    }
    return it == side.begin();
  }

  bool Reduce(uint64_t seqno, std::string ref, bool buy_sell, double price, double qty) {
    auto &side = buy_sell ? buy_ : sell_;
    int64_t prio = buy_sell ? -price : price;
    auto it = side.find(prio);
    if (it == side.end()) {
      return false;
    }

    it->second.qty -= qty; // reducing total level quantity
    it->second.seqno = seqno;

    if (it->second.qty <= 0) { // delete the whole level
      side.erase(it);
    }

    if(it->second.orders.find(ref) != it->second.orders.end()){
        it->second.orders[ref].qty -= qty;
    }

    if(it->second.orders[ref].qty == 0){
      it->second.orders.erase(ref);
    }
    return it == side.begin();
  }

  bool IsCrossed() {
    if (buy_.empty() || sell_.empty()) {
      return false;
    }
    return -buy_.rbegin()->first >= sell_.rbegin()->first;
  }

  void UnCross() {
    // Invalidates iterators
    auto bit = buy_.begin();
    auto sit = sell_.begin();
    while (bit != buy_.end() && sit != sell_.end() &&
           bit->second.price >= sit->second.price) {
      if (bit->second.seqno > sit->second.seqno) {
        sell_.erase(sit++);
      } else {
        buy_.erase(bit++);
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &out, const OrderBook &book) {
    out << "Buy:" << std::endl;
    for (auto it = book.buy_.rbegin(); it != book.buy_.rend(); it++) {
      out << it->second << std::endl;
    }
    out << "Sell:" << std::endl;
    for (auto it = book.sell_.rbegin(); it != book.sell_.rend(); it++) {
      out << it->second << std::endl;
    }
    return out;
  }

public:
  boost::container::flat_map<int64_t, Level, std::greater<int64_t>> buy_, sell_;
  void *data_ = nullptr;
};



class OrderBookManager {

public:
  QueueManager& queue_manager; 
  static constexpr int16_t NOBOOK = std::numeric_limits<int16_t>::max();
  static constexpr int16_t MAXBOOK = std::numeric_limits<int16_t>::max();

  //static_assert(sizeof(Order) == 16, "");

public:
  OrderBookManager(size_t size_hint, bool all_orders ,bool all_books, QueueManager& qu)
      : queue_manager(qu), all_orders_(all_orders), all_books_(all_books),
        symbols_(16384, 0){}

  ~OrderBookManager() {
    if (orders_.bucket_count() > size_hint_) {
      std::cerr << "WARNING bucket count " << orders_.bucket_count()
                << " greater than size hint " << size_hint_
                << ", recommend increasing size_hint" << std::endl;
    }
  }

  OrderBook &Subscribe(std::string instrument, void *data = NULL) {
    if (instrument.size() < 8) {
      instrument.insert(instrument.size(), 8 - instrument.size(), ' ');
    }
    uint64_t symbol = __builtin_bswap64(
        *reinterpret_cast<const uint64_t *>(instrument.data()));

    auto it = symbols_.find(symbol);
    if (it != symbols_.end()) {
      return books_[it->second]; 
    }

    if (books_.size() == MAXBOOK) {
      throw std::runtime_error("too many subscriptions");
    }

    books_.push_back(OrderBook());
    symbols_.emplace(symbol, books_.size() - 1);

    OrderBook &book = books_.back();
    book.SetUserData(data);
    return book;
  }

  void Add(uint64_t seqno, std::string ref, bool buy_sell, double qty,uint64_t symbol, double price) {
    auto it = symbols_.find(symbol);
    if (it == symbols_.end()) {
      if (!all_books_) {
        if (all_orders_) {
          orders_.emplace(ref, Order(price, qty, buy_sell, NOBOOK));
          std::cout << "sa" << std::endl;
        }
        return;
      }
      if (books_.size() == MAXBOOK) {
        // too many books
        return;
      }
      books_.push_back(OrderBook());
      it = symbols_.emplace(symbol, books_.size() - 1).first;
    }
    int16_t bookid = it->second;
    OrderBook &book = books_[bookid];
    orders_.emplace(ref, Order(price, qty, buy_sell, bookid)); // check
    Order orderr = Order(price,qty,buy_sell,bookid);
    bool top = book.Add(seqno,ref, buy_sell, price, qty, orderr);
    
  }

  void Reduce(uint64_t seqno, std::string ref, double qty) {
    auto oit = orders_.find(ref);
    if (oit == orders_.end()) {
      return; // if not found dont bother reducing it
    }

    Order &order = oit->second;
    if (order.bookid != NOBOOK) {
      OrderBook &book = books_[order.bookid];
      bool top = book.Reduce(seqno, ref, order.buy_sell, order.price, qty);
    }

    order.qty -= qty;
    if (order.qty <= 0) {
      orders_.erase(oit);
    }
  }

  void Delete(uint64_t seqno, std::string ref) {
    auto oit = orders_.find(ref);
    if (oit == orders_.end()) {
      return;
    }

    Order &order = oit->second;
    if (order.bookid != NOBOOK) {
      OrderBook &book = books_[order.bookid];
      bool top = book.Reduce(seqno, ref, order.buy_sell, order.price, order.qty);
    }

    orders_.erase(oit);
  }



  size_t Size() const { return orders_.size(); }


  void handle_coinbase_change(std::string ref, bool buy_sell, double price, double qty, OrderBook& book){

    auto &side = buy_sell ? book.buy_ : book.sell_;
    int64_t prio = buy_sell ? -price : price;
    
    auto it = side.find(prio);
    // if no price found : 
    if (it == side.end()) {
    	// do nothing 
  	}
    else{ // if price found then replace the order with corresponding size
    	//it->second.orders.insert({ref,order});
    	//it->second.qty += qty;
    	//it->second.seqno = seqno;
    	//book.seq = seqno;

    	if(it->second.orders.find(ref) != it->second.orders.end()){
    		Order o = it->second.orders[ref];
    		o.qty = qty;
    	}

    }
  }

  void reset_book(OrderBook& book, std::string symbol){
    ssl::context ctx1{ssl::context::tlsv12_client};
    ctx1.set_verify_mode(ssl::verify_peer);
    ctx1.set_default_verify_paths();
    
    // clear everything    
    orders_.clear();
    book.buy_.clear();
    book.sell_.clear();

    net::io_context ioc;
    auto coinbaseapi = std::make_shared<coinbaseAPI>(ioc.get_executor(),ctx1,ioc);
    json response = coinbaseapi->get_snapshot(symbol,3);
    uint64_t no = response["sequence"].get<std::uint64_t>();
    book.seq = no;

    bool buy_sell;
    std::string p;
    std::string q;
    std::string id;
    
    double price;
    double qty;

    for (const auto& bid : response["bids"]) {
        buy_sell = true;
        id = bid[2].get<std::string>();
        p = bid[0].get<std::string>();
        q = bid[1].get<std::string>();

        price = std::stod(p);
        qty = std::stod(q);
        //std::cout << "buys" <<  std::endl;
        //std::cout << "id : " << bid[2] << std::endl;
        //std::cout << "price : " << bid[0] << std::endl;
        //std::cout << "size : " << bid[1] << std::endl;
        Order order = Order(price,qty,buy_sell,books_.size() - 1);
        book.Add(no,id, buy_sell, price, qty, order);

    }

    for (const auto& ask : response["asks"]) {
        buy_sell = false;
        id = ask[2].get<std::string>();
        p = ask[0].get<std::string>();
        q = ask[1].get<std::string>();
        price = std::stod(p);
        qty = std::stod(q);
        
        //std::cout << "buys" <<  std::endl;
        //std::cout << "id : " << ask[2] << std::endl;
        //std::cout << "price : " << ask[0] << std::endl;
        //std::cout << "size : " << ask[1] << std::endl;
        Order order = Order(price,qty,buy_sell,books_.size() - 1);
        book.Add(no,id, buy_sell, price, qty, order);
    }

    //std::cout << book << std::endl;
  }

  void handle_coinbase(json message){
    std::string symbol = message["symbol"].get<std::string>();
    OrderBook& book = Subscribe(symbol);

    uint64_t sequence= message["sequence"].get<std::uint64_t>();


    if(book.seq == 0){ // first
    	reset_book(book,symbol);
    }
    if(book.seq == -1){
    	reset_book(book,symbol);
    }
    if (sequence <= book.seq){ // older messages
    	return;
    }
    if(sequence >= book.seq+1){
    	reset_book(book,symbol);
    }

    uint64_t seq;
    bool buy_sell;
    std::string p;
    std::string r;
    double remaining_size;
    double price;
    std::string orderid;
    if(message["type"] == "open"){
      std::cout << "open" << std::endl;
    	if(message["side"] == "buy"){
    		seq = message["sequence"].get<std::uint64_t>();
    		orderid = message["order_id"].get<std::string>();
    		p = message["price"].get<std::string>();
        price = std::stod(p);
    		r = message["remaining_size"].get<std::string>();
        remaining_size = std::stod(r);
    		
        buy_sell = true;
    		Add(seq,orderid,buy_sell,remaining_size,654,price); // fix the symbol
    	}
    	else{

    		seq = message["sequence"].get<std::uint64_t>();
    		std::string orderid = message["order_id"].get<std::string>();
    		p = message["price"].get<std::string>();
    		price = std::stod(p);
        r = message["remaining_size"].get<std::string>();
        remaining_size = std::stod(r);
        buy_sell = false;
    		Add(seq,orderid,buy_sell,remaining_size,654,price); // fix the symbol
    	}
    }

    else if(message["type"] == "change"){
      std::cout << "change" << std::endl;
      if(message["reason"] == "STP"){

      	if(message.contains("new_size")){
     			r = message["new_size"].get<std::string>();
          remaining_size = std::stod(r);
      	}else{ 
      		std::cout << "INVALID CHANGE MESSAGE" << std::endl;   
      		return;
      	}
      	if(message["side"] == "buy"){
      		seq = message["sequence"].get<std::uint64_t>();
      		orderid = message["order_id"].get<std::string>();
      		p = message["price"].get<std::string>();
          price = std::stod(p);
      		buy_sell = true;
      		handle_coinbase_change(orderid,buy_sell,price,remaining_size,book);
      		book.seq = seq;
      	}else{
      		orderid = message["order_id"].get<std::string>();
      		p = message["price"].get<std::string>();
          price = std::stod(p);
      		buy_sell = false;
      		handle_coinbase_change(orderid,buy_sell,price,remaining_size,book);
      		seq = message["sequence"].get<std::uint64_t>();
      		book.seq = seq;
      	}
      }
    }
	else if (message["type"] == "match"){
      std::cout << "match" << std::endl;

		if(message["side"] == "buy"){
			orderid = message["order_id"].get<std::string>();
			seq = message["sequence"].get<std::uint64_t>();
			p = message["price"].get<std::string>();
      price = std::stod(p);
			r = message["size"].get<std::string>();
      remaining_size = std::stod(r);
			Reduce(seq, orderid, remaining_size);
      book.seq = seq;
		}
	}

  else if(message["type"] == "done"){
    std::cout << "done" << std::endl;

    orderid = message["order_id"].get<std::string>();

    seq = message["sequence"].get<std::uint64_t>();

    p = message["price"].get<std::string>();

    price = std::stod(p);

    r = message["remaining_size"].get<std::string>();

    remaining_size = std::stod(r);
    Reduce(seq, orderid, remaining_size);
    book.seq = seq;
    
  }
  //std::cout << book << std::endl;
	
}

  void run(){
    while (true){
      if(queue_manager.COINBASE_INPUT_QUEUE.get().front() != nullptr){
        //std::cout << "popping" << std::endl;
        json msg = *queue_manager.COINBASE_INPUT_QUEUE.get().front();
        handle_coinbase(msg);
        //std::cout << msg << std::endl;
        queue_manager.COINBASE_INPUT_QUEUE.get().pop();
      }
    }
  }


private:
  // Non-copyable
  OrderBookManager(const OrderBookManager &) = delete;
  OrderBookManager &operator=(const OrderBookManager &) = delete;

  struct Hash { 
    size_t operator()(uint64_t h) const noexcept {
      h ^= h >> 33;
      h *= 0xff51afd7ed558ccd;
      h ^= h >> 33;
      h *= 0xc4ceb9fe1a85ec53;
      h ^= h >> 33;
      return h;
    }
  };

  size_t size_hint_ = 0;
  bool all_orders_ = false;
  bool all_books_ = false;

  std::vector<OrderBook> books_;
  //std::unordered_map<std::string, uint16_t, Hash> symbols_;
  HashMap<uint64_t, uint16_t, Hash> symbols_;
  // std::unordered_map<uint64_t, Order, Hash> orders_;
  //HashMap<std::string, Order, Hash> orders_;

  std::unordered_map<std::string, Order> orders_;
};