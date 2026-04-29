#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype> 

struct Command {
    std::string name;
    std::vector<std::string> args;
};

class CommandParser {
    public: 
        Command parse(const std::string& input) const;
};