
#include <gtest/gtest.h>
#include "LRUCache.h"
#include <stdexcept>

// TEST 1: Successful insertion and lookup
TEST(LRUCacheTest, ReturnsInsertedValue)
{

    /**
     *  AAA pattern
     *  A - Arrage 
     *  A - Act 
     *  A - Assert
     */

    //  Arrange
    LRUCache cache(2);

    // Act
    cache.put(10, 100);

    // Assert
    EXPECT_EQ(100, cache.get(10));
    std:: cout << "continue\n";
    EXPECT_TRUE(10 != cache.get(10));
    std:: cout << "continue\n";
    EXPECT_FALSE(100 != cache.get(10));
    // ASSERT_EQ(10, cache.get(10));
    std:: cout << "continue\n";
    EXPECT_EQ(100, cache.get(10));
    std:: cout << "END\n";


}

// TEST 2: Missing key returns -1
TEST(LRUCacheTest, MissingKeyReturnsMinusOne)
{
    LRUCache cache(2);

    EXPECT_EQ(cache.get(99), -1);
}

// TEST 3: Updating an existing key
TEST(LRUCacheTest, UpdatesExistingKey)
{
    LRUCache cache(2);

    cache.put(10, 100);
    cache.put(10, 200);

    EXPECT_EQ(cache.get(10), 200);
}

// TEST 4: Zero capacity
TEST(LRUCacheTest, ZeroCapacityDoesNotStoreEntries)
{
    LRUCache cache(0);

    cache.put(10, 100);

    EXPECT_EQ(cache.get(10), -1);
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

    EXPECT_EQ(cache.get(20), -1);

    EXPECT_EQ(cache.get(10), 100);
    EXPECT_EQ(cache.get(30), 300);
    EXPECT_EQ(cache.get(40), 400);
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

    EXPECT_EQ(cache.get(20), -1);
    EXPECT_EQ(cache.get(10), 100);
    EXPECT_EQ(cache.get(30), 300);
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

    EXPECT_EQ(cache.get(20), -1);
    EXPECT_EQ(cache.get(10), 111);
    EXPECT_EQ(cache.get(30), 300);
}

// TEST 8: Missing lookup does not affect recency
TEST(LRUCacheTest, MissingLookupDoesNotChangeRecency)
{
    LRUCache cache(2);

    cache.put(10, 100);
    cache.put(20, 200);

    EXPECT_EQ(cache.get(99), -1);

    // Key 10 was already LRU and should be evicted.
    cache.put(30, 300);

    EXPECT_EQ(cache.get(10), -1);
    EXPECT_EQ(cache.get(20), 200);
    EXPECT_EQ(cache.get(30), 300);
}

// TEST 9: Capacity one
TEST(LRUCacheTest, CapacityOneEvictsPreviousEntry)
{
    LRUCache cache(1);

    cache.put(10, 100);
    cache.put(20, 200);

    EXPECT_EQ(cache.get(10), -1);
    EXPECT_EQ(cache.get(20), 200);
}

// TEST 10: Negative capacity validation
TEST(LRUCacheTest, NegativeCapacityThrowsException)
{
    EXPECT_THROW(
        LRUCache(-1),
        std::invalid_argument
    );
}
