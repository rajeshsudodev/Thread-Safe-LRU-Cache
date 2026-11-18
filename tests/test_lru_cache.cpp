
#include <gtest/gtest.h>
#include "LRUCache.h"

#include <stdexcept>
#include <optional>
#include <string>
#include <thread>
#include <vector>
#include <atomic>
// ============================================================
// EXISTING INTEGER TESTS
// ============================================================

// TEST 1: Successful insertion and lookup
TEST(LRUCacheTest, ReturnsInsertedValue)
{
    LRUCache<int, int> cache(3);

    cache.put(10, 100);

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 100);
}

// TEST 2: Missing key returns std::nullopt
TEST(LRUCacheTest, MissingKeyReturnsNullopt)
{
    LRUCache<int, int> cache(2);

    auto result = cache.get(99);

    EXPECT_FALSE(result.has_value());
}

// TEST 3: Updating an existing key
TEST(LRUCacheTest, UpdatesExistingKey)
{
    LRUCache<int, int> cache(2);

    cache.put(10, 100);
    cache.put(10, 200);

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 200);
}

// TEST 4: Zero capacity
TEST(LRUCacheTest, ZeroCapacityDoesNotStoreEntries)
{
    LRUCache<int, int> cache(0);

    cache.put(10, 100);

    EXPECT_FALSE(cache.get(10).has_value());
}

// TEST 5: LRU eviction order
TEST(LRUCacheTest, EvictsLeastRecentlyUsedKey)
{
    LRUCache<int, int> cache(3);

    cache.put(10, 100);
    cache.put(20, 200);
    cache.put(30, 300);

    cache.get(10);

    cache.put(40, 400);

    EXPECT_FALSE(cache.get(20).has_value());

    ASSERT_TRUE(cache.get(10).has_value());
    EXPECT_EQ(cache.get(10).value(), 100);

    ASSERT_TRUE(cache.get(30).has_value());
    EXPECT_EQ(cache.get(30).value(), 300);

    ASSERT_TRUE(cache.get(40).has_value());
    EXPECT_EQ(cache.get(40).value(), 400);
}

// TEST 6: Repeated access updates recency
TEST(LRUCacheTest, RepeatedAccessKeepsKeyMostRecentlyUsed)
{
    LRUCache<int, int> cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    cache.get(10);
    cache.get(10);

    cache.put(30, 300);

    EXPECT_FALSE(cache.get(20).has_value());

    ASSERT_TRUE(cache.get(10).has_value());
    EXPECT_EQ(cache.get(10).value(), 100);

    ASSERT_TRUE(cache.get(30).has_value());
    EXPECT_EQ(cache.get(30).value(), 300);
}

// TEST 7: Updating an existing key also updates recency
TEST(LRUCacheTest, UpdatingKeyMakesItMostRecentlyUsed)
{
    LRUCache<int, int> cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    cache.put(10, 111);

    cache.put(30, 300);

    EXPECT_FALSE(cache.get(20).has_value());

    ASSERT_TRUE(cache.get(10).has_value());
    EXPECT_EQ(cache.get(10).value(), 111);

    ASSERT_TRUE(cache.get(30).has_value());
    EXPECT_EQ(cache.get(30).value(), 300);
}

// TEST 8: Missing lookup does not affect recency
TEST(LRUCacheTest, MissingLookupDoesNotChangeRecency)
{
    LRUCache<int, int> cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    EXPECT_FALSE(cache.get(99).has_value());

    cache.put(30, 300);

    EXPECT_FALSE(cache.get(10).has_value());

    ASSERT_TRUE(cache.get(20).has_value());
    EXPECT_EQ(cache.get(20).value(), 200);

    ASSERT_TRUE(cache.get(30).has_value());
    EXPECT_EQ(cache.get(30).value(), 300);
}

// TEST 9: Capacity one
TEST(LRUCacheTest, CapacityOneEvictsPreviousEntry)
{
    LRUCache<int, int> cache(1);

    cache.put(10, 100);
    cache.put(20, 200);

    EXPECT_FALSE(cache.get(10).has_value());

    ASSERT_TRUE(cache.get(20).has_value());
    EXPECT_EQ(cache.get(20).value(), 200);
}

// TEST 10: Negative capacity validation
TEST(LRUCacheTest, NegativeCapacityThrowsException)
{
    EXPECT_THROW(
        (LRUCache<int, int>(-1)),
        std::invalid_argument
    );
}

// TEST 11: Stored value -1 is valid
TEST(LRUCacheTest, StoredMinusOneIsValidValue)
{
    LRUCache<int, int> cache(2);

    cache.put(10, -1);

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), -1);
}

// TEST 12: Missing key and stored -1 are distinguishable
TEST(LRUCacheTest, DistinguishesMissingKeyFromStoredMinusOne)
{
    LRUCache<int, int> cache(2);

    cache.put(10, -1);

    auto existingResult = cache.get(10);
    auto missingResult = cache.get(99);

    EXPECT_TRUE(existingResult.has_value());
    EXPECT_FALSE(missingResult.has_value());

    ASSERT_TRUE(existingResult.has_value());
    EXPECT_EQ(existingResult.value(), -1);
}


// ============================================================
// NEW GENERIC TEMPLATE TESTS
// ============================================================

// TEST 13: Integer key + string value
TEST(LRUCacheGenericTest, SupportsIntegerKeyAndStringValue)
{
    LRUCache<int, std::string> cache(3);

    cache.put(1, "one");
    cache.put(2, "two");

    auto result = cache.get(2);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "two");
}

// TEST 14: String key + string value
TEST(LRUCacheGenericTest, SupportsStringKeyAndStringValue)
{
    LRUCache<std::string, std::string> cache(3);

    cache.put("one", "first");
    cache.put("two", "second");

    auto result = cache.get("two");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "second");
}

// TEST 15: String key + double value
TEST(LRUCacheGenericTest, SupportsStringKeyAndDoubleValue)
{
    LRUCache<std::string, double> cache(3);

    cache.put("C++", 17.0);
    cache.put("Python", 3.14);

    auto result = cache.get("C++");

    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(result.value(), 17.0);
}

// TEST 16: Generic key eviction
TEST(LRUCacheGenericTest, EvictsLeastRecentlyUsedGenericKey)
{
    LRUCache<std::string, std::string> cache(2);

    cache.put("one", "first");
    cache.put("two", "second");

    // Make "one" the most recently used.
    cache.get("one");

    // "two" should be evicted.
    cache.put("three", "third");

    EXPECT_FALSE(cache.get("two").has_value());

    ASSERT_TRUE(cache.get("one").has_value());
    EXPECT_EQ(cache.get("one").value(), "first");

    ASSERT_TRUE(cache.get("three").has_value());
    EXPECT_EQ(cache.get("three").value(), "third");
}

// TEST 17: Generic value update
TEST(LRUCacheGenericTest, UpdatesExistingGenericValue)
{
    LRUCache<int, std::string> cache(2);

    cache.put(10, "old value");
    cache.put(20, "another value");

    cache.put(10, "updated value");

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "updated value");
}


// TEST 18: Concurrency put by multiple threads
TEST(LRUCacheTest, ConcurrentPutFromTwoThreads)
{
    LRUCache<int, int> cache(10);

    std::thread t1([&cache]()
    {
        cache.put(10, 100);
    });

    std::thread t2([&cache]()
    {
        cache.put(20, 200);
    });

    t1.join();
    t2.join();

    ASSERT_EQ(cache.get(10), std::optional<int>(100));
    ASSERT_EQ(cache.get(20), std::optional<int>(200));
}


TEST(LRUCacheTest, concurrentPutFromMultipleThreads)
{
    constexpr int threadCount = 8;
    constexpr int operationsPerThread = 100;

    LRUCache<int , int> cache(threadCount * operationsPerThread);

    std:: vector<std:: thread> threads;

    for(int threadId {0}; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([&cache, threadId]()
        {


            for(int i {0}; i < operationsPerThread; ++i)
            {
                int key  = threadId * operationsPerThread + i;
                cache.put(key, key *10);
            }
        });
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    for(int key = 0; key < threadCount * operationsPerThread; ++key )
    {
        auto result = cache.get(key);

        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(key*10 , result.value());
    }


}

// TEST 20: Concurrent readers and writers
TEST(LRUCacheTest, ConcurrentGetAndPut)
{
    constexpr int threadCount = 4;
    constexpr int operationsPerThread = 100;

    LRUCache<int, int> cache(500);

    std::atomic<int> successfulWrites{0};
    std::atomic<int> failedLookups{0};

    std::vector<std::thread> threads;

    // Initialize some entries.
    for (int i = 0; i < 100; ++i)
    {
        cache.put(i, i * 10);
    }

    // Two writer threads.
    for (int t = 0; t < 2; ++t)
    {
        threads.emplace_back([&cache, &successfulWrites, t]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                int key = 100 + t * operationsPerThread + i;
                cache.put(key, key * 10);
                successfulWrites.fetch_add(1);
            }
        });
    }

    // Two reader threads.
    for (int t = 0; t < 2; ++t)
    {
        threads.emplace_back([&cache, &failedLookups]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                auto result = cache.get(i);

                if (!result.has_value())
                {
                    failedLookups.fetch_add(1);
                }
            }
        });
    }

    // Wait for every thread.
    for (auto& thread : threads)
    {
        thread.join();
    }

    // Validate completed writes.
    EXPECT_EQ(successfulWrites.load(), 200);

    // Initial entries must remain because capacity is 500
    // and the test inserts only 300 distinct keys.
    EXPECT_EQ(failedLookups.load(), 0);

    // Verify final values.
    for (int i = 0; i < 100; ++i)
    {
        auto result = cache.get(i);

        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), i * 10);
    }
}



 // TEST 21: Multiple threads update the same key
TEST(LRUCacheTest, ConcurrentUpdatesToSameKey)
{
    constexpr int threadCount = 4;
    constexpr int updatesPerThread = 1000;

    LRUCache<int, int> cache(10);

    cache.put(10, 0);

    std::vector<std::thread> threads;

    for (int threadId = 1; threadId <= threadCount; ++threadId)
    {
        threads.emplace_back([&cache, threadId]()
        {
            for (int i = 0; i < updatesPerThread; ++i)
            {
                cache.put(10, threadId);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());

    EXPECT_GE(result.value(), 1);
    EXPECT_LE(result.value(), threadCount);
}

 // TEST 22: Capacity remains bounded during concurrent insertion
TEST(LRUCacheTest, ConcurrentPutRespectsCapacity)
{
    constexpr int threadCount = 8;
    constexpr int operationsPerThread = 1000;
    constexpr int capacity = 100;

    LRUCache<int, int> cache(capacity);

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (int threadId = 0; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([&cache, threadId]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                int key = threadId * operationsPerThread + i;
                cache.put(key, key * 10);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    // Exactly 'capacity' distinct keys were inserted,
    // but the cache must retain no more than capacity entries.
    int retainedEntries = 0;

    for (int key = 0; key < threadCount * operationsPerThread; ++key)
    {
        if (cache.get(key).has_value())
        {
            ++retainedEntries;
        }
    }

    EXPECT_EQ(retainedEntries, capacity);
}



TEST(LRUCacheTest, HighContentionConcurrentPut)
{
    constexpr int threadCount = 16;
    constexpr int operationsPerThread = 1000;
    constexpr int capacity = 1000;

    LRUCache<int, int> cache(capacity);

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (int threadId = 0; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([threadId, &cache]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                int key = threadId * operationsPerThread + i;
                cache.put(key, key * 10);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    int retainedEntries = 0;

    for (int key = 0; key < threadCount * operationsPerThread; ++key)
    {
        if (cache.get(key).has_value())
        {
            ++retainedEntries;
        }
    }

    EXPECT_EQ(retainedEntries, capacity);
}



TEST(LRUCacheTest, ConcurrentOperationsWithSmallCapacity)
{
    constexpr int threadCount = 8;
    constexpr int operationsPerThread = 1000;
    constexpr int capacity = 2;

    LRUCache<int, int> cache(capacity);

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (int threadId = 0; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([threadId, &cache]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                int key = (i + threadId) % 10;

                cache.put(key, key * 10);
                auto result = cache.get(key);

                if (i % 5 == 0)
                {
                    cache.put(key, key * 100);
                }
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    int retainedEntries = 0;

    for (int key = 0; key < 10; ++key)
    {
        if (cache.get(key).has_value())
        {
            ++retainedEntries;
        }
    }

    EXPECT_LE(retainedEntries, capacity);
}



TEST(LRUCacheTest, ConcurrentRepeatedSameKeyOperations)
{
    constexpr int threadCount = 8;
    constexpr int operationsPerThread = 2000;

    LRUCache<int, int> cache(10);

    cache.put(10, 0);

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (int threadId = 0; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([threadId, &cache]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                cache.put(10, threadId * operationsPerThread + i);

                auto result = cache.get(10);

                EXPECT_TRUE(result.has_value());
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    auto finalValue = cache.get(10);

    ASSERT_TRUE(finalValue.has_value());
    EXPECT_GE(*finalValue, 0);
    EXPECT_LT(*finalValue, threadCount * operationsPerThread);
}



TEST(LRUCacheTest, IndependentCacheInstancesWorkConcurrently)
{
    constexpr int threadCount = 4;
    constexpr int operationsPerThread = 1000;

    LRUCache<int, int> cacheA(4000);
    LRUCache<int, int> cacheB(4000);

    std::vector<std::thread> threads;

    for (int threadId = 0; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([threadId, &cacheA, &cacheB]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                int key = threadId * operationsPerThread + i;

                cacheA.put(key, key * 10);
                cacheB.put(key, key * 100);

                auto valueA = cacheA.get(key);
                auto valueB = cacheB.get(key);

                EXPECT_TRUE(valueA.has_value());
                EXPECT_TRUE(valueB.has_value());
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    for (int key = 0; key < threadCount * operationsPerThread; ++key)
    {
        auto valueA = cacheA.get(key);
        auto valueB = cacheB.get(key);

        ASSERT_TRUE(valueA.has_value());
        ASSERT_TRUE(valueB.has_value());

        EXPECT_EQ(*valueA, key * 10);
        EXPECT_EQ(*valueB, key * 100);
    }
}



TEST(LRUCacheTest, ConcurrentOperationsCompleteWithoutDeadlock)
{
    constexpr int threadCount = 8;
    constexpr int operationsPerThread = 2000;

    LRUCache<int, int> cache(100);

    std::atomic<int> completedThreads{0};

    std::vector<std::thread> threads;

    for (int threadId = 0; threadId < threadCount; ++threadId)
    {
        threads.emplace_back([threadId, &cache, &completedThreads]()
        {
            for (int i = 0; i < operationsPerThread; ++i)
            {
                int key = (threadId * 17 + i) % 200;

                cache.put(key, i);
                auto result = cache.get(key);

                if (i % 10 == 0)
                {
                    cache.put(key, i * 2);
                }
            }

            completedThreads.fetch_add(1);
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(completedThreads.load(), threadCount);
}
