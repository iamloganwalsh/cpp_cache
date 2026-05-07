#include "../include/commanddispatcher.hpp"

CommandDispatcher::CommandDispatcher(DataStore& data_store)
    : data_store_(data_store) {}

void CommandDispatcher::execute(const Command& cmd) {

    if (cmd.name == "SET") {
        if (cmd.args.size() != 2) {
            std::cout << "ERROR: SET requires 2 arguments (key value)\n";
            return;
        }
        data_store_.set(cmd.args[0], cmd.args[1]);
    }
    else if (cmd.name == "GET") {
        if (cmd.args.size() != 1) {
            std::cout << "ERROR: GET requires 1 argument (key)\n";
            return;
        }

        std::optional<std::string> value = data_store_.get(cmd.args[0]);

        if (value) {
            std::cout << *value << std::endl;
        } else {
            std::cout << "KEY: " + cmd.name + " NOT FOUND" << std::endl;
        }
    }
    else if (cmd.name == "DEL") {
        if (cmd.args.size() != 1) {
            std::cout << "ERROR: DEL requires 1 argument (key)\n";
            return;
        }

        int result = data_store_.del(cmd.args[0]);

        if (result) {
            std::cout << "KEY: " + cmd.args[0] + " HAS BEEN DELETED" << std::endl;
        } else {
            std::cout << "KEY: " + cmd.args[0] + " NOT FOUND" << std::endl;
        }
    }
    else if (cmd.name == "EXISTS") {
        if (cmd.args.size() != 1) {
            std::cout << "ERROR: EXISTS requires 1 argument (key)\n";
            return;
        }

        int result = data_store_.exists(cmd.args[0]);

        if (result) {
            std::cout << "KEY: " + cmd.args[0] + " EXISTS" << std::endl;
        } else {
            std::cout << "KEY: " + cmd.args[0] + " DOESN'T EXIST" << std::endl;
        }
    }
    else if (cmd.name == "DUMP") {
        if (cmd.args.size() != 0) {
            std::cout << "ERROR: DUMP takes no arguments\n";
            return;
        }

        std::vector<std::tuple<std::string, std::string>> dump = data_store_.data_dump();
        for (const auto& [k, v] : dump) {
            std::cout << k << " = " << v << "\n";
        }
    } 
    else if (cmd.name == "KEYS") {
        if (cmd.args.size() != 0) {
            std::cout << "ERROR: KEYS takes no arguments\n";
            return;
        }

        std::vector<std::string> dump = data_store_.keys();
        for (const auto& k : dump) {
            std::cout << k << "\n";
        }
    }
    else if (cmd.name == "EXIT") {
        return;
    }
    else if (cmd.name == "HELP") {
        data_store_.help();
    }
    else if (cmd.name == "SETEX") {
        if (cmd.args.size() != 3) {
            std::cout << "ERROR: SETEX requires 3 arguments (key value ttl_seconds)\n";
            return;
        }

        try {
            int ttl_seconds = std::stoi(cmd.args[2]);

            if (ttl_seconds <= 0) {
                std::cout << "ERROR: TTL must be greater than 0\n";
                return;
            }

            data_store_.setex(
                cmd.args[0],
                cmd.args[1],
                ttl_seconds
            );
        }
        catch (const std::exception&) {
            std::cout << "ERROR: TTL must be a valid integer. Current TTL may be too large.\n";
        }
    }
    else if (cmd.name == "TTL") {
        if (cmd.args.size() != 1) {
            std::cout << "ERROR: TTL requires 1 argument (key)\n";
            return;
        }

        int result = data_store_.ttl(cmd.args[0]);

        if (result == -1) {
            std::cout << "Key: " + cmd.args[0] + " not found or has expired.\n";
            return;
        }
        else if (result == -2) {
            std::cout << "Key: " + cmd.args[0] + " exists but has no TTL.\n";
            return;
        }

        std::cout << "Key: " << cmd.args[0] << " TTL: " << result << "\n";
    }
    else {
        std::cout << "ERROR: " + cmd.name + " NOT RECOGNISED AS A COMMAND.\n";
        std::cout << "Run 'HELP' to see a list of available commands.\n";
        return;
    }
}