#include "../include/datastore.hpp"
#include "../include/commandparser.hpp"
#include "../include/commanddispatcher.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main() {
    DataStore ds;
    CommandParser cmdpar;
    Command cmd;
    CommandDispatcher cmddis(ds);

    while (true) {
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

