cmake_minimum_required(VERSION 3.20)
project(cpp_cache)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where headers are
include_directories(include)

# Define executable + source files
add_executable(cpp_cache
    src/main.cpp
    src/datastore.cpp
    src/commandparser.cpp
    src/commanddispatcher.cpp
)