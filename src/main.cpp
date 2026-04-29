#include "../include/datastore.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void error(const std::vector<std::string>& tokens) {
    std::cout << "ERROR: " + tokens[0] + " INSTRUCTION HAS WRONG AMOUNT OF INPUTS" << std::endl;
}

std::vector<std::string> split(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;

    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
    DataStore ds;

    while (true) {
        std::string line;
        std::cout << "> ";
        std::getline(std::cin, line);

        auto tokens = split(line);

        std::string command = tokens[0];

        if (command == "SET") {
            if (tokens.size() != 3) {
                error(tokens);
                continue;
            }
            ds.set(tokens[1], tokens[2]);

        } else if (command == "GET") {
            if (tokens.size() != 2) {
                error(tokens);
                continue;
            }
            std::optional<std::string> value = ds.get(tokens[1]);

            if (value) {
                std::cout << *value << std::endl;
            } else {
                std::cout << "KEY: " + tokens[1] + " NOT FOUND" << std::endl;
            }

        } else if (command == "DEL") {
            if (tokens.size() != 2) {
                error(tokens);
                continue;
            }
            int result = ds.del(tokens[1]);

            if (result == 1) {
                std::cout << "KEY: " + tokens[1] + "HAS BEEN DELETED" << std::endl;
            } else {
                std::cout << "KEY: " + tokens[1] + " NOT FOUND" << std::endl;
            }

        } else if (command == "EXISTS") {
            if (tokens.size() != 2) {
                error(tokens);
                continue;
            }

            int result = ds.exists(tokens[1]);

            if (result == 1) {
                std::cout << "KEY: " + tokens[1] + " EXISTS" << std::endl;
            } else {
                std::cout << "KEY: " + tokens[1] + " NOT FOUND" << std::endl;
            }

        } else if (command == "DUMP") {
            if (tokens.size() != 1) {
                error(tokens);
                continue;
            }
            auto dump = ds.data_dump();
            for (const auto& [k, v] : dump) {
                std::cout << k << " = " << v << "\n";
            }

        } else if (command == "KEYS") {
            if (tokens.size() != 1) {
                error(tokens);
                continue;
            }
            auto key_vector = ds.keys();
            for (const auto& k : key_vector) {
                std::cout << k << "\n";
            }
        } else {
            std::cout << "COMMAND NOT FOUND: " + tokens[0] << std::endl;
        }
    }
}

