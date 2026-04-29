#include "../include/datastore.hpp"

void DataStore::set(const std::string& key, const std::string& value) {
    data_store_[key] = value;
}

std::optional<std::string> DataStore::get(const std::string& key) const {
    auto iterator = data_store_.find(key);

    if (iterator != data_store_.end()) {
        return iterator->second;
    }

    return std::nullopt;
}

bool DataStore::del(const std::string& key) {
    return data_store_.erase(key) == 1;
}

bool DataStore::exists(const std::string& key) const {
    return data_store_.find(key) != data_store_.end();
}

std::vector<std::tuple<std::string, std::string>> DataStore::data_dump() const {
    std::vector<std::tuple<std::string, std::string>> data_collection;

    for (const auto& [key, value] : data_store_) {
        data_collection.push_back(std::make_tuple(key, value));
    }

    return data_collection;
}

std::vector<std::string> DataStore::keys() const {
    std::vector<std::string> key_vector;

    for (const auto& [key, value] : data_store_) {
        key_vector.push_back(key);
    }

    return key_vector;
}