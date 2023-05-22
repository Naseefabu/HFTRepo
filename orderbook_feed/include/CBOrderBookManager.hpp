#pragma once

#include "HashMap.hpp"
#include <boost/container/flat_map.hpp>
#include "SPSCQueue.hpp"
#include "types.hpp"
#include <iostream>

#include <nlohmann/json.hpp>

using namespace rigtorp;
using json = nlohmann::json;

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

class OrderBook { 
public:
  struct Level {
    int64_t price = 0;
    int64_t qty = 0;

    friend std::ostream &operator<<(std::ostream &out, const Level &level) {
      out << "Level(" << level.price << ", " << level.qty << ", " 
          << ")";
      return out;
    }
  };

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

  void Add(bool buy_sell, int64_t price, int64_t qty) {

    auto &side = buy_sell ? buy_ : sell_;
    int64_t prio = buy_sell ? -price : price;
    auto it = side.find(prio);
    if (it == side.end()) {
      auto it = side.insert(std::make_pair(prio, Level())).first;
      it->second.price = price;
      it->second.qty = qty;
    }else{
      side.erase(it);
      Add(buy_sell,price,qty);
    }
  }

  void Delete(bool buy_sell, int64_t price) {
    auto &side = buy_sell ? buy_ : sell_;
    int64_t prio = buy_sell ? -price : price;
    auto it = side.find(prio);
    if (it == side.end()) { // no price
      return;
    }
    side.erase(it);
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

private:
  boost::container::flat_map<int64_t, Level, std::greater<int64_t>> buy_, sell_;
  void *data_ = nullptr;
};

class CoinbaseOrderBookManager {

public:
  std::reference_wrapper<SPSCQueue<json>> que;


  static constexpr int16_t NOBOOK = std::numeric_limits<int16_t>::max();
  static constexpr int16_t MAXBOOK = std::numeric_limits<int16_t>::max();

  struct Order {
    int64_t price = 0;
    int32_t qty = 0;
    bool buy_sell = 0;
    int16_t bookid = NOBOOK;

    Order(int64_t price, int32_t qty, int16_t buy_sell, int16_t bookid)
        : price(price), qty(qty), buy_sell(buy_sell), bookid(bookid) {}
    Order() {}
  };

  static_assert(sizeof(Order) == 16, "");

public:
  CoinbaseOrderBookManager(size_t size_hint, bool all_orders ,bool all_books , SPSCQueue<json>& qu)
      : all_orders_(all_orders), all_books_(all_books),
        symbols_(16384, 0),
        que(qu),
        orders_(size_hint, std::numeric_limits<uint64_t>::max()) {
    size_hint_ = orders_.bucket_count();
  }

  ~CoinbaseOrderBookManager() {
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

  void Add(bool buy_sell, int32_t qty, uint64_t symbol, int64_t price) {
    auto it = symbols_.find(symbol);
    if (it == symbols_.end()) {
      if (books_.size() == MAXBOOK) {
        // too many books
        return;
      }
      books_.push_back(OrderBook());
      it = symbols_.emplace(symbol, books_.size() - 1).first;
    }
    int16_t bookid = it->second;
    OrderBook &book = books_[bookid];

    book.Add(buy_sell, price, qty);
 
  }

  size_t Size() const { return orders_.size(); }

  void handle_snapshot(std::string symbol, json message){
    
    OrderBook& book = Subscribe(symbol);
    bool buy_sell = true;
    for (const auto& bid : message["bids"]) {
      double price = std::stod(bid[0].get<std::string>());
      double size = std::stod(bid[1].get<std::string>()); 
      book.Add(buy_sell,price,size);
    }
    buy_sell = false;
    for (const auto& ask : message["asks"]) {
      double price = std::stod(ask[0].get<std::string>());
      double size = std::stod(ask[1].get<std::string>());
      book.Add(buy_sell,price,size);
    }
  }

  void handle_deltas(std::string symbol, json message){
    OrderBook& book = Subscribe(symbol);
    std::cout << book << std::endl;
    bool buy_sell;
    for (const auto& change : message["changes"]) {
      if(change[0]=="buy"){
        buy_sell = true;
        double price = std::stod(change[1].get<std::string>());
        double size = std::stod(change[2].get<std::string>());
        if(size==0){ // delete
          book.Delete(buy_sell,price);
        }else{
          book.Add(buy_sell,price,size);
        }
      }
      else{
        buy_sell = false;
        double price = std::stod(change[1].get<std::string>());
        double size = std::stod(change[2].get<std::string>());
        if(size==0){ // delete
          book.Delete(buy_sell,price);
        }else{
          book.Add(buy_sell,price,size);
        }
      }
    }
    std::cout << book << std::endl;

  }

  void run(){
    while (true){
      if((que.get()).front() != nullptr){
        std::cout << "popping" << std::endl;
        json msg = *(que.get()).front();
        if(msg["type"] == "snapshot"){
          handle_snapshot(msg["symbol"],msg);
        }
        else{
          handle_deltas(msg["symbol"],msg);
        }
        (que.get()).pop();
      }
    }
  }


private:
  // Non-copyable
  CoinbaseOrderBookManager(const CoinbaseOrderBookManager &) = delete;
  CoinbaseOrderBookManager &operator=(const CoinbaseOrderBookManager &) = delete;

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
  // std::unordered_map<uint64_t, uint16_t, Hash> symbols_;
  HashMap<uint64_t, uint16_t, Hash> symbols_;
  // std::unordered_map<uint64_t, Order, Hash> orders_;
  HashMap<uint64_t, Order, Hash> orders_;
};