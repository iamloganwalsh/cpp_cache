#include "../include/datastore.hpp"
#include "../include/commandparser.hpp"
#include "../include/commanddispatcher.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main() {
    std::string cache_name;
    std::cout << "Enter cache name: ";
    std::getline(std::cin, cache_name);

    DataStore ds(cache_name);
    CommandParser cmdpar;
    Command cmd;
    CommandDispatcher cmddis(ds);

    while (cmd.name != "EXIT") {
        std::string line;
        std::cout << "> ";
        std::getline(std::cin, line);        

        cmd = cmdpar.parse(line);
        if (cmd.name.empty()) {
            std::cout << "ERROR: COMMAND IS EMPTY\n";
            continue;
        }

        cmddis.execute(cmd);
    }
}

