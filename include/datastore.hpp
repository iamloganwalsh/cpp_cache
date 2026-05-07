#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

class DataStore {
public:
    DataStore(const std::string& cache_name);
    ~DataStore();
    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    std::vector<std::tuple<std::string, std::string>> data_dump();
    void setex(const std::string& key, const std::string& value, int ttl_seconds);
    int ttl(const std::string& key);
    std::vector<std::string> keys();
    void help();
private:
    std::unordered_map<std::string, std::string> data_store_;
    std::ofstream log_file_;
    std::string cache_name_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> expiry_map_;
    bool is_replaying_ = false;

    bool isExpired(const std::string& key);
    void removeExpiredKey(const std::string& key);
    void cleanupExpiredKeys();

    void loadFromSnapshot(); 
    void updateSnapshot(); 
    void loadFromLog();
    void appendToLog(const std::string& entry); 
};