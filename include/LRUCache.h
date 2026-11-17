#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list> // Doubly Linked List 
#include <unordered_map> // HashMap
#include <utility>   // std:: pair
#include <optional>

// LRU Cache = Fixed Capacity + Recency Tracking + Eviction
/**
 * Fixed-Capacity Least Recently Used cache.
 * Front of list = MRU [Most Recently used]
 * Back of list = LRU [Least Recently used]
 */

class LRUCache
{
private:
 
    int capacity_; 

    // Stores key-value pairs.
    std:: list< std:: pair<int , int>> lru;

    // Maps each key to its corresponding list node.
    std:: unordered_map<int , std:: list< std:: pair<int , int>>:: iterator> cacheMap;
public:
    
    /**
     * Creates an LRU cache with specified capacity .
     * Negative capacity throws std:: invalid_argument.
     * Zero capacity is allowed.
     */
    explicit LRUCache(int capacity); // Explicit -> only direct initialization allowed

    /**
     * Retrives the value associated with the key.
     * Successfully lookup updates key's recency order.
     * Return -1 when the key does not found.
     */
    std:: optional<int> get (int key);

    /**
     * Insert new key-pair or updates existing key 
     * The inserted or updated key becomes most recently used (at the front of list)
     * when full, the least recently used entry is evicted ([key , value ] removed from both cacheMap and  lruList)
     */
    void put (int key , int value);

};

#endif