cmake_minimum_required(VERSION 3.20)
project(cpp_cache)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# ----------------------------
# Core library (shared logic)
# ----------------------------
add_library(core
    src/datastore.cpp
    src/commandparser.cpp
    src/commanddispatcher.cpp
)

target_include_directories(core PUBLIC include)

# ----------------------------
# Main executable
# ----------------------------
add_executable(cpp_cache
    src/main.cpp
)

target_link_libraries(cpp_cache core)

# ----------------------------
# Tests executable
# ----------------------------
add_executable(tests
    tests/test_main.cpp
)

target_link_libraries(tests core)