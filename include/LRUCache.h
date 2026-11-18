#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <iostream>
#include <list> // Doubly Linked List
#include <unordered_map> // HashMap
#include <utility>   // std:: pair
#include <optional>
#include <stdexcept>
#include <cstddef>
#include <mutex>
#include <iterator>

// using std:: cout, std:: endl; pollutes the namespace of every single

// LRU Cache = Fixed Capacity + Recency Tracking + Eviction
/**
 * Fixed-Capacity Least Recently Used cache.
 * Front of list = MRU [Most Recently used]
 * Back of list = LRU [Least Recently used]
 */

template<typename Key, typename Value>
class LRUCache
{
private:

    int capacity_;

    // Stores key-value pairs.
    std:: list< std:: pair<Key , Value>> lru;

    // Maps each key to its corresponding list node.
    std:: unordered_map<Key , typename std:: list< std:: pair<Key, Value>>:: iterator> cacheMap;

    std:: mutex mutex_;
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
    //  * Return -1 when the key does not found.
     * Return std:: nullopt when the key is not found.
     */
    std:: optional<Value> get (const Key& key);

    /**
     * Insert new key-pair or updates existing key
     * The inserted or updated key becomes most recently used (at the front of list)
     * when full, the least recently used entry is evicted ([key , value ] removed from both cacheMap and  lruList)
     */
    void put (const Key& key , const Value& value);


    /**
     * Copy and Move semantics Disabled due to mutex injection
     */

     // Copy Disable
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    // Move Disable
    LRUCache& operator=(LRUCache&&) = delete;
    LRUCache(LRUCache&&) = delete;

};




// Member initialization list
template<typename Key, typename Value>
LRUCache<Key, Value>::LRUCache(int capacity) : capacity_(capacity) {

    /**
     * Validating Cache Capacity
     * Negative handling
     */
    if(capacity < 0)
    {
        throw std::invalid_argument("Cache Capacity cannot be negative");
    }

}


/**
 * insertion operation
 * insert key - value if not exists
 * update value if already exists
 */
template<typename Key, typename Value>
void LRUCache<Key, Value>::put(const Key& key, const Value& value)
{
    /* CRITICAL SECTION */

    std:: lock_guard<std:: mutex> lock(mutex_);

    // Zero capacity initialization
    if(static_cast<std:: size_t>(capacity_) == 0) {
        // std:: cout << "[Warning] CacheMap Capacity is 0\n";
        return;
    }



    // get It
    auto mapIt = cacheMap.find(key);

    // check if item already exists
    if(mapIt != cacheMap.end() )
    {
        // update the existing value inplace
        auto listIt = mapIt->second;  // returns the iteraotr of pair<key , value>
        listIt->second = value;

        // Move the existing node to the front [ Recency tracking ]
        lru.splice(lru.begin() , lru, listIt);  // splice(target position , list , source position)

        return;
    }

    // If not exists insert it into lruList and cacheMap


    // Exception Safety

    // Insert new node at front
    lru.emplace_front(key , value);

    try
    {
        // attempt insert in hashmap
        auto [mapIt , inserted] = cacheMap.emplace(key , lru.begin());
        // auto MapIt = cacheMap.emplace(key , lru.begin());

        // if insert not successed , remove then new list node
        if(!inserted){
            lru.pop_front();
            return;
        }

        // if successed move on ..
    }
    catch(...)
    {
        // Map insertion failed : roll back
        lru.pop_front();

        // rethrow the original exception
        throw;
    }


    /**
     Check whether the cache is full.
     Retrieve the key from the last list node.
     Remove that key from cacheMap.
     Remove the last node from lru.
     Insert the new key-value pair at the front.
     Update cacheMap
     */

    // If no space left

    // after inserting a node then capacity becomes full
    if(cacheMap.size() > static_cast<std:: size_t>(capacity_))
    {
        // get It
        // auto listIt = --(lru.end()); // returns last pair<T, T> iterator
        // std:: prev -> <iterator>
        auto listIt = std:: prev(lru.end()); // returns last pair<T, T> iterator
        // --listIt;  // points to the last node [tail] lruList<pair>
        // const auto& listIt = lru.back();  // returns last pair<T, T> copy

         // remove pair <key , value>  from cache
        // cacheMap.erase(listIt->first);  // returns key , works for --(lru.end());
        cacheMap.erase(listIt->first);  // returns key , works for lru.back();

        // remove last node pair<key, value> form lruList
        lru.pop_back();
    }

    // Insert new key-value at the front [ Recency tracking ]
    // lru.emplace_front(key , value);

    // Store the key and its list iterator in the cacheMap
    // cacheMap.emplace(key, lru.begin());
}


/**
 *
 1. Search for the key.

 2. If not found, return std:: optnull

 3. Retrieve the list iterator.

 4. Save the value.

 5. Move the node to the front using splice().

 6. Return the saved value.
 */
template<typename Key, typename Value>
std:: optional<Value> LRUCache<Key, Value>:: get(const Key& key)
{

    /* CRITICAL SECTION */
    std:: lock_guard<std:: mutex> lock(mutex_);


    // get It
    auto mapIt = cacheMap.find(key);

    // if key not exists in cacheMap
    if(mapIt == cacheMap.end())
    {
        // std:: cout << "key not found\n";
        return std::nullopt;
    }

    // If key exists
    auto listIt = mapIt->second;  // return list<pair> :: iterator
    auto value = listIt->second;  // save value

    // Move the node to front of lru list
    // auto pos = lru.begin();
    // auto src = listIt;
    // lru.splice(pos, lru , src);
    lru.splice(lru.begin() , lru , listIt);

    // return listIt->second;  // return value
    return value;
}
#endif