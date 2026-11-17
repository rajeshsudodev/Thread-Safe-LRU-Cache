
#include <gtest/gtest.h>
#include "LRUCache.h"

#include <stdexcept>
#include <optional>
#include <string>

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
