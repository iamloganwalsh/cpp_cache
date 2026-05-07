#include <iostream>
#include <random>
#include <string>
#include <filesystem>

#include "../include/datastore.hpp"
#include "../include/commandparser.hpp"
#include "../include/commanddispatcher.hpp"

namespace fs = std::filesystem;

int tests_passed = 0;
int tests_failed = 0;

std::string generate_cache_name() {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(32);

    for (int i = 0; i < 32; i++) {
        result += charset[dist(rng)];
    }

    return result;
}

void cleanup_cache(const std::string& name) {
    fs::remove("storage/" + name + ".snp");
    fs::remove("storage/" + name + ".log");
}

class TestCache {
public:
    std::string name;

    TestCache() {
        name = generate_cache_name();
        cleanup_cache(name);
    }

    ~TestCache() {
        cleanup_cache(name);
    }
};

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "TEST FAILED: " << #cond \
                  << " at line " << __LINE__ << "\n"; \
        tests_failed++; \
    } else { \
        tests_passed++; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    if (!((a) == (b))) { \
        std::cerr << "TEST FAILED: " << #a << " == " << #b \
                  << " (got " << (a) << " vs " << (b) << ") at line " << __LINE__ << "\n"; \
        tests_failed++; \
    } else { \
        tests_passed++; \
    } \
} while (0)

void test_set_get() {
    TestCache cache;
    DataStore ds(cache.name);

    ds.set("a", "1");

    auto v = ds.get("a");

    EXPECT_TRUE(v.has_value());
    if (v.has_value()) {
        EXPECT_EQ(v.value(), "1");
    }
}

void test_override_value() {
    TestCache cache;
    DataStore ds(cache.name);

    ds.set("a", "1");
    ds.set("a", "2");

    auto v = ds.get("a");
    EXPECT_TRUE(v.has_value());
    if (v.has_value()) {
        EXPECT_EQ(v.value(), "2");
    }
}

void test_delete() {
    TestCache cache;
    DataStore ds(cache.name);

    ds.set("a", "1");
    ds.del("a");

    EXPECT_TRUE(!ds.exists("a"));
}

void test_exists() {
    TestCache cache;
    DataStore ds(cache.name);

    ds.set("x", "10");

    EXPECT_TRUE(ds.exists("x"));
}

void test_keys() {
    TestCache cache;
    DataStore ds(cache.name);

    ds.set("a", "1");
    ds.set("b", "2");

    auto keys = ds.keys();

    EXPECT_EQ(keys.size(), 2);
}

void test_parser() {
    CommandParser parser;

    Command cmd = parser.parse("SET name Logan");

    EXPECT_EQ(cmd.name, "SET");
    EXPECT_EQ(cmd.args.size(), 2);
    if (cmd.args.size() == 2) {
        EXPECT_EQ(cmd.args[0], "name");
        EXPECT_EQ(cmd.args[1], "Logan");
    }
}

void test_dispatch_set_get() {
    TestCache cache;
    DataStore ds(cache.name);

    CommandParser parser;
    CommandDispatcher dispatcher(ds);

    dispatcher.execute(parser.parse("SET name Logan"));

    auto v = ds.get("name");

    EXPECT_TRUE(v.has_value());
    if (v.has_value()) {
        EXPECT_EQ(v.value(), "Logan");
    }
}

void test_dispatch_delete() {
    TestCache cache;
    DataStore ds(cache.name);

    CommandParser parser;
    CommandDispatcher dispatcher(ds);

    dispatcher.execute(parser.parse("SET a 1"));
    dispatcher.execute(parser.parse("DEL a"));

    EXPECT_TRUE(!ds.exists("a"));
}

void test_sequence() {
    TestCache cache;
    DataStore ds(cache.name);

    CommandParser parser;
    CommandDispatcher dispatcher(ds);

    dispatcher.execute(parser.parse("SET a 1"));
    dispatcher.execute(parser.parse("SET b 2"));
    dispatcher.execute(parser.parse("DEL a"));
    dispatcher.execute(parser.parse("SET c 3"));

    EXPECT_TRUE(!ds.exists("a"));
    EXPECT_TRUE(ds.exists("b"));
    EXPECT_TRUE(ds.exists("c"));
}

// ===============================
// MAIN
// ===============================
int main() {
    std::cerr << "Running tests...\n";

    test_set_get();
    test_override_value();
    test_delete();
    test_exists();
    test_keys();
    test_parser();
    test_dispatch_set_get();
    test_dispatch_delete();
    test_sequence();

    std::cerr << "\n=====================\n";
    std::cerr << "TEST SUMMARY\n";
    std::cerr << "=====================\n";
    std::cerr << "PASSED: " << tests_passed << "\n";
    std::cerr << "FAILED: " << tests_failed << "\n";

    return tests_failed == 0 ? 0 : 1;
}