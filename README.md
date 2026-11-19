# Thread-Safe LRU Cache

A fixed-capacity, thread-safe **Least Recently Used (LRU) Cache** implemented in Modern C++17 using `std::list`, `std::unordered_map`, and `std::mutex`.

This project is a C++ systems programming project focused on understanding data structures, object lifetime, exception safety, concurrency, synchronization, and thread-safe API design.

The implementation evolves from a generic single-threaded LRU Cache into a thread-safe cache protected by a per-instance mutex.

## 1. Features

- Fixed-capacity LRU cache.
- Generic `Key` and `Value` support.
- Average O(1) key lookup.
- Insert and update key-value pairs.
- Automatic Least Recently Used (LRU) eviction.
- Recency tracking on successful lookup.
- Zero-capacity handling.
- Negative-capacity validation.
- `std::optional<Value>` return type for safe lookup.
- Thread-safe `get()` and `put()` operations.
- Per-instance mutex synchronization.
- Concurrent access protection.
- Exception-aware insertion with rollback on failed map insertion.
- Automated unit testing using GoogleTest.
- Concurrent workload validation.
- ThreadSanitizer validation.

## 2. Data Structures

Two STL containers are used to implement the LRU Cache.

### `std::list<std::pair<Key, Value>>`

- Stores key-value pairs.
- Front represents Most Recently Used (MRU).
- Back represents Least Recently Used (LRU).
- Supports efficient recency updates using `std::list::splice()`.
- Provides stable iterators for stored elements.

### `std::unordered_map<Key, list::iterator>`

- Maps each key to its corresponding list iterator.
- Provides average O(1) key lookup.
- Allows direct access to the corresponding list node.
- Avoids searching the list during lookup and eviction.

Together, these data structures support efficient lookup, insertion, updating, and eviction.

### Internal Relationship

The hash map stores iterators pointing to nodes in the linked list.

```text
          unordered_map
       Key → List Iterator
              |
              v
        std::list
   [MRU] → [Entry] → [Entry] → [LRU]
```

Both containers must remain consistent throughout every cache operation.

## 3. Project Structure

```text
Thread-Safe-LRU-Cache/
│
├── include/
│   └── LRUCache.h
│
├── src/
│   └── main.cpp
│
├── tests/
│   └── test_lru_cache.cpp
│
├── build/
├── build-tsan/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## 4. Public API

| Operation                         | Description                                       |
|-----------------------------------|---------------------------------------------------|
| `LRUCache<Key, Value>(capacity)`  | Creates a cache with the specified capacity       |
| `get(key)`                        | Retrieves a value and updates recency             |
| `put(key, value)`                 | Inserts a new entry or updates an existing entry  |

### API Behaviour

- Negative capacity throws `std::invalid_argument`.
- Zero capacity is allowed; insertions are ignored.
- A successful `get()` updates the entry's recency.
- A missing key returns `std::nullopt`.
- A stored value of `-1` is distinguishable from a missing key.
- Updating an existing key makes it the most recently used.
- Inserting into a full cache evicts the least recently used entry.
- `get()` returns `std::optional<Value>` by value.
- Copy and move construction and assignment are disabled.

### Generic Type Support

The cache supports different key and value types, subject to the requirements of the underlying STL containers.

Examples:

```cpp
LRUCache<int, int> cache1(100);

LRUCache<std::string, std::string> cache2(100);

LRUCache<int, std::string> cache3(100);
```

## 5. Complexity Analysis

| Operation             | Average Time Complexity   |
|-----------------------|---------------------------|
| Key lookup            | O(1)                      |
| Successful `get()`    | O(1)                      |
| New insertion         | O(1) amortized            |
| Existing-key update   | O(1)                      |
| LRU eviction          | O(1)                      |

**Note:** Hash-table operations have O(n) worst-case complexity. Insertion may also trigger rehashing.

The single-mutex synchronization strategy serializes cache operations within an individual instance. Consequently, actual throughput depends on lock contention and workload characteristics.

## 6. Thread Safety and Synchronization

The cache uses one `std::mutex` per cache instance to protect its shared mutable state.

### Synchronization Strategy

- Each cache instance owns its own mutex.
- `get()` and `put()` acquire the mutex using `std::lock_guard<std::mutex>`.
- The internal `std::list` and `std::unordered_map` are protected by the same mutex.
- Cache operations are serialized within an individual instance.
- Independent cache instances have independent mutexes.

### Thread-Safety Guarantees

- Multiple threads can safely call `get()` and `put()` concurrently on the same cache instance.
- Concurrent insertion, lookup, update, and eviction operations are synchronized.
- The relationship between the hash map and linked list is protected as one logical unit.
- `get()` returns a value by copy through `std::optional<Value>`, avoiding references or iterators into cache-managed storage after releasing the mutex.

### API and Lifetime Restrictions

- The cache object must remain alive until all threads using it have completed their operations.
- Thread safety of externally shared objects referenced or owned by `Value` is the caller's responsibility.
- The cache mutex does not automatically protect external data accessed by callers.
- Copy and move operations are disabled.

### Performance Consideration

This implementation prioritizes correctness and simplicity through a single mutex per cache instance.

All operations on the same instance are serialized while holding the mutex. This may limit throughput under high contention.

Sharded locking and other scalability improvements are possible future enhancements.

## 7. Requirements

- C++17 compatible compiler.
- CMake 3.16 or later.
- GoogleTest.
- POSIX threads support for the current Linux/WSL development environment.

### Ubuntu / WSL Dependencies

```bash
sudo apt update
sudo apt install cmake g++ libgtest-dev
```

## 8. Build Instructions

Configure the project:

```bash
cmake -S . -B build
```

Build the application and test executable:

```bash
cmake --build build -j2
```

Run the application:

```bash
./build/lru_cache
```

## 9. Testing and Validation

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

The test suite currently contains 27 test cases covering:

**Functional testing**
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
11. Stored `-1` value handling.
12. Generic key and value types.
13. Generic LRU eviction and update behaviour.

**Concurrent testing**
14. Concurrent insertion from two threads.
15. Concurrent insertion from multiple threads.
16. Concurrent `get()` and `put()`.
17. Concurrent updates to the same key.
18. Concurrent capacity validation.
19. High-contention concurrent insertion.
20. Concurrent operations with small capacity.
21. Concurrent repeated same-key operations.
22. Independent cache instances operating concurrently.
23. Concurrent operations completing without deadlock.



### ThreadSanitizer Validation

ThreadSanitizer (TSan) is used to detect data races during observed test executions.

A separate sanitizer-enabled build is maintained to avoid modifying the normal build configuration.

Configure:

```bash
cmake -S . -B build-tsan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=thread -g -O1 -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread"
```

Build:

```bash
cmake --build build-tsan -j2
```

Run the tests:

```bash
ctest --test-dir build-tsan --output-on-failure
```

**Note:** A successful TSan run indicates that no data races were detected in the observed executions. It does not prove the absence of races in every possible execution.

## 10. Project Development Roadmap

### Stage 1 — Generic Single-Threaded LRU Cache

**Status: Completed**

- Implemented the LRU data structure.
- Introduced generic `Key` and `Value` support.
- Implemented safe lookup using `std::optional<Value>`.
- Handled edge cases and object lifetime.
- Added functional GoogleTest coverage.
- Published the Stage 1 implementation.

### Stage 2 — Thread Safety using `std::mutex`

<!-- **Status: Concurrent validation completed; documentation and publishing in progress.** -->

- Introduced per-instance mutex synchronization.
- Protected shared mutable state.
- Implemented concurrent functional tests.
- Validated concurrent workloads.
- Executed tests under ThreadSanitizer.
- Completed reliability testing.


### Future Enhancements

Potential future improvements include:

- Sharded locking for improved concurrency.
- Performance benchmarking.
- Lock contention analysis.
- Further scalability experiments.
