#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class DataStore {
public:
    DataStore(const std::string& cache_name);
    ~DataStore();
    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool del(const std::string& key);
    bool exists(const std::string& key) const;
    std::vector<std::tuple<std::string, std::string>> data_dump() const;
    std::vector<std::string> keys() const;
private:
    std::unordered_map<std::string, std::string> data_store_;
    std::ofstream log_file_;
    std::string cache_name_;
    bool is_replaying_ = false;

    void loadFromSnapshot(); 
    void updateSnapshot(); 
    void loadFromLog();
    void appendToLog(const std::string& entry); 
};