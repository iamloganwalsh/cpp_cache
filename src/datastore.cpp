#include "../include/datastore.hpp"

void DataStore::set(const std::string& key, const std::string& value) {
    data_store_[key] = value;
    if (!is_replaying_) {
        appendToLog("SET " + key + " " + value);
    }
}

std::optional<std::string> DataStore::get(const std::string& key) const {
    auto iterator = data_store_.find(key);

    if (iterator != data_store_.end()) {
        return iterator->second;
    }

    return std::nullopt;
}

bool DataStore::del(const std::string& key) {
    bool erased = data_store_.erase(key) == 1;

    if (erased && !is_replaying_) {
        appendToLog("DEL " + key);
    }

    return erased;
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

void DataStore::loadFromSnapshot() {
    std::ifstream file("storage/" + cache_name_ + ".snp");

    if (!file.is_open()) return;

    std::string line;

    while (std::getline(file, line)) {
        auto pos = line.find(':');

        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        data_store_[key] = value;
    }
}

void DataStore::updateSnapshot() {
    std::ofstream file("storage/" + cache_name_ + ".snp", std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Failed to write to storage/" + cache_name_ + ".snp\n";
        return;
    }

    for (const auto& [key, value] : data_store_) {
        file << key << ":" << value << "\n";
    }

    // Clear the log
    std::ofstream ofs("storage/" + cache_name_ + ".log", std::ios::trunc);
    ofs.close(); 
}

void DataStore::loadFromLog() {
    std::ifstream file("storage/" + cache_name_ + ".log");

    if (!file.is_open()) return;

    is_replaying_ = true;

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string command;
        iss >> command;

        if (command == "SET") {
            std::string key, value;
            iss >> key >> value;
            data_store_[key] = value;
        }
        else if (command == "DEL") {
            std::string key;
            iss >> key;
            data_store_.erase(key);
        }
    }

    is_replaying_ = false;
}

void DataStore::appendToLog(const std::string& entry) {
    log_file_ << entry << "\n";
    log_file_.flush();
}

DataStore::DataStore(const std::string& cache_name)
    : cache_name_(cache_name)
{
    loadFromSnapshot();
    loadFromLog();
    log_file_.open("storage/" + cache_name_ + ".log", std::ios::app);
}

DataStore::~DataStore() {
    updateSnapshot();
    if (log_file_.is_open()) {
        log_file_.close();
    }
}
