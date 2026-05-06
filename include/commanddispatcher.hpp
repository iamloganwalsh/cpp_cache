#pragma once

#include "commandparser.hpp"
#include "datastore.hpp"

class CommandDispatcher {
public:
    CommandDispatcher(DataStore& data_store);

    void execute(const Command& cmd);

private:
    DataStore& data_store_;
};