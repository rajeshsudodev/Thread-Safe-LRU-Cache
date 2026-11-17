
#include <gtest/gtest.h>
#include "LRUCache.h"
#include <stdexcept>
#include <optional>

// TEST 1: Successful insertion and lookup
TEST(LRUCacheTest, ReturnsInsertedValue)
{
    // Arrange
    LRUCache cache(3);

    // Act
    cache.put(10, 100);

    auto result = cache.get(10);

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 100);
}

// TEST 2: Missing key returns std::nullopt
TEST(LRUCacheTest, MissingKeyReturnsNullopt)
{
    LRUCache cache(2);

    auto result = cache.get(99);

    EXPECT_FALSE(result.has_value());
}

// TEST 3: Updating an existing key
TEST(LRUCacheTest, UpdatesExistingKey)
{
    LRUCache cache(2);

    cache.put(10, 100);
    cache.put(10, 200);

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 200);
}

// TEST 4: Zero capacity
TEST(LRUCacheTest, ZeroCapacityDoesNotStoreEntries)
{
    LRUCache cache(0);

    cache.put(10, 100);

    EXPECT_FALSE(cache.get(10).has_value());
}

// TEST 5: LRU eviction order
TEST(LRUCacheTest, EvictsLeastRecentlyUsedKey)
{
    LRUCache cache(3);

    cache.put(10, 100);
    cache.put(20, 200);
    cache.put(30, 300);

    // Make key 10 the most recently used.
    cache.get(10);

    // Key 20 should be evicted.
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
    LRUCache cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    cache.get(10);
    cache.get(10);

    // Key 20 should be evicted.
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
    LRUCache cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    // Updating key 10 should make it MRU.
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
    LRUCache cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    EXPECT_FALSE(cache.get(99).has_value());

    // Key 10 was already LRU and should be evicted.
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
    LRUCache cache(1);

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
        LRUCache(-1),
        std::invalid_argument
    );
}

// TEST 11: Stored value -1 is valid
TEST(LRUCacheTest, StoredMinusOneIsValidValue)
{
    LRUCache cache(2);

    cache.put(10, -1);

    auto result = cache.get(10);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), -1);
}

// TEST 12: Missing key and stored -1 are distinguishable
TEST(LRUCacheTest, DistinguishesMissingKeyFromStoredMinusOne)
{
    LRUCache cache(2);

    cache.put(10, -1);

    auto existingResult = cache.get(10);
    auto missingResult = cache.get(99);

    EXPECT_TRUE(existingResult.has_value());
    EXPECT_FALSE(missingResult.has_value());

    ASSERT_TRUE(existingResult.has_value());
    EXPECT_EQ(existingResult.value(), -1);
}
