#include "../include/commandparser.hpp"

Command CommandParser::parse(const std::string& input) const{
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return {"", {}};
    }

    // Convert command name to uppercase
    std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::toupper);

    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    Command cmd = {tokens[0], args};

    return cmd;
}
