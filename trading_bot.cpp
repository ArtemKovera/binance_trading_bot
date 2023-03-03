#include "trading_bot.h"


    TradingBot::TradingBot(const std::string& apiKey, const std::string& secretKey) :
        m_apiKey(apiKey),
        m_secretKey(secretKey) 
    {}

    json TradingBot::getTickerPrice(const std::string& symbol) 
    {
        std::string url = "https://api.binance.com/api/v3/ticker/price?symbol=" + symbol;
        std::string response = get(url);
        return json::parse(response);
    }

    json TradingBot::getOrderBook(const std::string& symbol, int limit) 
    {
        std::string url = "https://api.binance.com/api/v3/depth?symbol=" + symbol + "&limit=" + std::to_string(limit);
        std::string response = get(url);
        return json::parse(response);
    }

    json TradingBot::placeOrder(const std::string& symbol, const std::string& side, double quantity, double price) 
    {
        std::string url = "https://api.binance.com/api/v3/order/test";
        std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        std::string query = "symbol=" + symbol + "&side=" + side + "&type=LIMIT&timeInForce=GTC&quantity=" + std::to_string(quantity) + "&price=" + std::to_string(price) + "&timestamp=" + timestamp;
        std::string signature = hmac_sha256(query, m_secretKey);
        std::string response = post(url, query + "&signature=" + signature, { "X-MBX-APIKEY: " + m_apiKey });
        return json::parse(response);
    }


    std::string TradingBot::get(const std::string& url) 
    {
        std::string response;
        CURL* curl = curl_easy_init();
        if (curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        return response;
    }

    std::string TradingBot::post(const std::string& url, const std::string& data, const std::vector<std::string>& headers) 
    {
        std::string response;
        CURL* curl = curl_easy_init();
        if (curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            struct curl_slist* headerList = NULL;
            for (auto header : headers) 
            {
                headerList = curl_slist_append(headerList, header.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
            CURLcode res = curl_easy_perform(curl);
            curl_slist_free_all(headerList);
            curl_easy_cleanup(curl);
        }
        return response;
    }

    size_t TradingBot::writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) 
    {
        std::string* response = (std::string*)userdata;
        response->append(ptr, size * nmemb);
        return size * nmemb;
    }

    std::string TradingBot::hmac_sha256(const std::string& data, const std::string& key) 
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        HMAC_CTX* hmac = HMAC_CTX_new();
        HMAC_Init_ex(hmac, key.c_str(), key.length(), EVP_sha256(), NULL);
        HMAC_Update(hmac, (const unsigned char*)data.c_str(), data.length());
        unsigned int length = SHA256_DIGEST_LENGTH;
        HMAC_Final(hmac, hash, &length);
        HMAC_CTX_free(hmac);
        std::stringstream ss;
        ss << std::hex;
        for (unsigned char i : hash) 
        {
            ss << std::setw(2) << std::setfill('0') << (int)i;
        }
        return ss.str();
    }
