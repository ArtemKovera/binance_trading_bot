#include "trading_bot.h"

int main() 
{
    std::string apiKey = "YOUR_API_KEY";
    std::string secretKey = "YOUR_SECRET_KEY";
    TradingBot api(apiKey, secretKey);
    json ticker = api.getTickerPrice("BTCUSDT");
    std::cout << "Current Bitcoin price: " << ticker["price"] << std::endl;
    json orderBook = api.getOrderBook("BTCUSDT");
    std::cout << "Asks: " << orderBook["asks"] << std::endl;
    std::cout << "Bids: " << orderBook["bids"] << std::endl;
    json order = api.placeOrder("BTCUSDT", "BUY", 0.01, 50000);
    std::cout << "Order placed: " << order << std::endl;
    return 0;
}

//g++ -o trading_bot main.cpp trading_bot.cpp -lcurl -lssl -lcrypto
