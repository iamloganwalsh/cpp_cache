# C++ Key-Value Cache

A lightweight Redis-inspired in-memory key-value cache built in modern C++23.  
The project focuses on clean system design, modular architecture, and incremental backend-style features such as persistence, command parsing, TTL support, and a command-line interface.

## Overview

This project implements a simple key-value store with a CLI interface, supporting core database-like operations. It is designed with a modular architecture consisting of:

- **DataStore** – core in-memory storage engine
- **CommandParser** – parses raw CLI input into structured commands
- **CommandDispatcher** – routes commands to the appropriate datastore operations
- **Persistence Layer** – snapshot + log-based recovery system
- **TTL System** – optional key expiration using lazy deletion

The goal of the project is to simulate core backend database design principles in a simplified environment.

## Requirements
- macOS (tested on Apple Clang / Xcode toolchain)
- OR Linux (expected compatibility with GCC/Clang)
- C++23 compatible compiler required
- CMake build system

## Build & Run

### Build project
```bash
./build.sh
```

### Run Test Cases
```
./run_tests.sh
```
Tests include:
- Unit tests for core DataStore operations
- Parser validation tests
- Integration tests for full command execution pipelines
- Basic persistence and TTL behavior validation

## Usage

## Startup
- On startup, you will be prompted to enter the name of the cache you want to interact with
- Entering a non-existent name will create a new cache

## Usage - Commands within cache

- SET <key> <value>
    - Store a value under a key

- GET <key>
    - Retrieve the value for a key

- DEL <key>
    - Delete a key from the cache

- EXISTS <key>
    - Check whether a key exists

- KEYS
    - List all keys currently stored

- DUMP
    - Print all key-value pairs

- SETEX <key> <value> <ttl_seconds>
    - Store a value with an expiration time
    - NOTE: TTL keys currently do not persist

- TTL <key>
    - Show remaining time-to-live for a key

- HELP
    - Display available commands

- EXIT
    - Save snapshot and exit the program