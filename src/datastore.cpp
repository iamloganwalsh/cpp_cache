#include "../include/datastore.hpp"

void DataStore::set(const std::string& key, const std::string& value) {
    data_store_[key] = value;

    if (!is_replaying_) {
        appendToLog("SET " + key + " " + value);
    }
}

std::optional<std::string> DataStore::get(const std::string& key) {
    isExpired(key);

    auto it = data_store_.find(key);

    if (it == data_store_.end()) {
        return std::nullopt;
    }

    return it->second;
}

bool DataStore::del(const std::string& key) {
    bool erased = data_store_.erase(key) == 1;

    if (erased) {
        expiry_map_.erase(key);
    }

    if (erased && !is_replaying_) {
        appendToLog("DEL " + key);
    }

    return erased;
}

bool DataStore::exists(const std::string& key) {
    isExpired(key);
    return data_store_.find(key) != data_store_.end();
}

std::vector<std::tuple<std::string, std::string>> DataStore::data_dump() {
    cleanupExpiredKeys();

    std::vector<std::tuple<std::string, std::string>> out;

    for (const auto& [key, value] : data_store_) {
        out.emplace_back(key, value);
    }

    return out;
}


void DataStore::setex(const std::string& key, const std::string& value, int ttl_seconds) {
    data_store_[key] = value;

    expiry_map_[key] =
        std::chrono::steady_clock::now() +
        std::chrono::seconds(ttl_seconds);

    if (!is_replaying_) {
        appendToLog("SETEX " + key + " " + value + " " + std::to_string(ttl_seconds));
    }
}

int DataStore::ttl(const std::string& key) {
    if (isExpired(key)) {
        return -1;
    }

    auto it = expiry_map_.find(key);
    bool key_exists = exists(key);

    if (it == expiry_map_.end() && key_exists) {
        return -2;
    }
    else if (!key_exists) {
        return -1;
    }

    auto remaining =
        std::chrono::duration_cast<std::chrono::seconds>(
            it->second - std::chrono::steady_clock::now()
        ).count();

    return static_cast<int>(remaining);
}

std::vector<std::string> DataStore::keys() {
    cleanupExpiredKeys();

    std::vector<std::string> out;

    for (const auto& [key, _] : data_store_) {
        out.push_back(key);
    }

    return out;
}

void DataStore::help() {
    std::cout << R"(

    ================ CACHE COMMANDS ================

    SET <key> <value>
        Store a value under a key.

    GET <key>
        Retrieve the value for a key.

    DEL <key>
        Delete a key from the cache.

    EXISTS <key>
        Check whether a key exists.

    KEYS
        List all keys currently stored.

    DUMP
        Print all key-value pairs.

    SETEX <key> <value> <ttl_seconds>
        Store a value with an expiration time.
        NOTE: TTL keys currently do not persist.

    TTL <key>
        Show remaining time-to-live for a key.

    HELP
        Display available commands.

    EXIT
        Save snapshot and exit the program.

    ================================================

    )";
}

bool DataStore::isExpired(const std::string& key) {
    auto it = expiry_map_.find(key);

    if (it == expiry_map_.end()) {
        return false;
    }

    if (std::chrono::steady_clock::now() >= it->second) {
        removeExpiredKey(key);
        return true;
    }

    return false;
}

void DataStore::cleanupExpiredKeys() {
    std::vector<std::string> to_remove;

    for (const auto& [key, _] : expiry_map_) {
        if (std::chrono::steady_clock::now() >= expiry_map_[key]) {
            to_remove.push_back(key);
        }
    }

    for (const auto& key : to_remove) {
        removeExpiredKey(key);
    }
}

void DataStore::removeExpiredKey(const std::string& key) {
    data_store_.erase(key);
    expiry_map_.erase(key);
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
        if (expiry_map_.find(key) != expiry_map_.end()) {
            continue; // skip TTL keys
        }

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
        else if (command == "SETEX") {
            std::string key, value;
            int ttl_seconds;

            iss >> key >> value >> ttl_seconds;

            setex(key, value, ttl_seconds);
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
