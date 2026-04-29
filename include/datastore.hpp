#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>

class DataStore {
public:
    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool del(const std::string& key);
    bool exists(const std::string& key) const;
    std::vector<std::tuple<std::string, std::string>> data_dump() const;
    std::vector<std::string> keys() const;
private:
    std::unordered_map<std::string, std::string> data_store_;
};