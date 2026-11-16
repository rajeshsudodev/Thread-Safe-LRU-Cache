# LRU-Cache
# LRU Cache — C++17

A fixed-capacity **Least Recently Used (LRU) Cache** implemented in C++17 using `std::list` and `std::unordered_map`.

This project is the first stage of a C++ systems programming project. It focuses on implementing and testing a correct single-threaded LRU Cache before introducing thread synchronization.

## 1. Features

- Fixed-capacity cache.
- Average O(1) key lookup.
- Insert and update key-value pairs.
- Automatic Least Recently Used (LRU) eviction.
- Recency tracking on successful lookup.
- Zero-capacity handling.
- Negative-capacity validation.
- Automated unit testing using GoogleTest.

## 2. Data Structures

Two STL containers are used:

**`std::list<std::pair<int, int>>`**

- Stores key-value pairs.
- Front represents Most Recently Used (MRU).
- Back represents Least Recently Used (LRU).
- Supports efficient recency updates using `std::list::splice()`.

**`std::unordered_map<int, list::iterator>`**

- Maps each key to its corresponding list iterator.
- Provides average O(1) key lookup.
- Allows direct access to the corresponding list node.

Together, these data structures support efficient lookup, insertion, updating, and eviction.

## 3. Project Structure

```text
Thread-Safe-LRU-Cache/
│
├── include/
│   └── LRUCache.h
│
├── src/
│   ├── LRUCache.cpp
│   └── main.cpp
│
├── tests/
│   └── test_lru_cache.cpp
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## 4. Public API

| Operation             | Description                                      |
|-----------------------|--------------------------------------------------|
| `LRUCache(capacity)`  | Creates a cache with the specified capacity      |
| `get(key)`            | Retrieves a value and updates recency            |
| `put(key, value)`     | Inserts a new entry or updates an existing entry |

### API Behaviour

- Negative capacity throws `std::invalid_argument`.
- Zero capacity is allowed; insertions are ignored.
- A successful `get()` updates the entry's recency.
- A missing key returns `-1`.
- Updating an existing key makes it the most recently used.
- Inserting into a full cache evicts the least recently used entry.

**Version 1 limitation:** A stored value of `-1` cannot be distinguished from a missing key using the current `get()` API. An improved `std::optional<Value>` API is planned for a later version.

## 5. Complexity Analysis

| Operation           | Average Time Complexity |
|---------------------|-------------------------|
| Key lookup          | O(1)                    |
| Successful `get()`  | O(1)                    |
| New insertion       | O(1)                    |
| Existing-key update | O(1)                    |
| LRU eviction        | O(1)                    |

*Note: Hash-table operations have O(n) worst-case complexity. Insertion may also trigger rehashing.*

## 6. Requirements

- C++17 compatible compiler
- CMake 3.16 or later
- GoogleTest

### Ubuntu / WSL Dependencies

```bash
sudo apt update
sudo apt install cmake g++ libgtest-dev
```

## 7. Build Instructions

Configure the project:

```bash
cmake -S . -B build
```

Build the application and test executable:

```bash
cmake --build build
```

Run the application:

```bash
./build/lru_cache
```

## 8. Unit Testing

This project uses GoogleTest for automated testing and CTest for test discovery and execution.

Run the GoogleTest executable directly:

```bash
./build/lru_cache_tests
```

Run all tests through CTest:

```bash
ctest --test-dir build --output-on-failure
```

### Current Test Coverage

The test suite contains 10 test cases covering:

1. Successful insertion and lookup.
2. Missing-key behaviour.
3. Updating existing keys.
4. Zero-capacity handling.
5. LRU eviction order.
6. Repeated access and recency.
7. Recency updates after modifying an existing key.
8. Missing lookup without changing recency.
9. Capacity-one behaviour.
10. Negative-capacity validation.

## 9. Current Project Scope

**Stage 1 — Single-threaded LRU Cache**

Completed implementation of the basic LRU data structure and its functional unit tests.

**Future development:**

- Stage 2 — Add one `std::mutex` to protect shared cache state.
- Stage 3 — Validate thread safety with concurrent tests and ThreadSanitizer.

The current implementation is single-threaded and is not yet thread-safe.
