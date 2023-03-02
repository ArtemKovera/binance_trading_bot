#ifndef TRADING_BOT_H
#define TRADING_BOT_H
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <openssl/hmac.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class BinanceAPI
{
public:
    BinanceAPI(const std::string& apiKey, const std::string& secretKey);

    json getTickerPrice(const std::string& symbol);

    json getOrderBook(const std::string& symbol, int limit = 100);

    json placeOrder(const std::string& symbol, const std::string& side, double quantity, double price);

private:
    std::string m_apiKey;
    std::string m_secretKey;

    std::string get(const std::string& url);

    std::string post(const std::string& url, const std::string& data, const std::vector<std::string>& headers);

    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

    std::string hmac_sha256(const std::string& data, const std::string& key);

};

#endif