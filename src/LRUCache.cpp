#include <iostream>
#include "LRUCache.h"
#include <stdexcept>


using std:: cout, std:: endl; // C++ 17

// Member initialization list
LRUCache::LRUCache(int capacity) : capacity_(capacity) {

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

void LRUCache::put(int key, int value)
{

    // Zero capacity initialization
    if(capacity_ == 0) {
        cout << "[Warning] CacheMap Capacity is 0\n";
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


    /**
    Check whether the cache is full.
    Retrieve the key from the last list node.
    Remove that key from cacheMap.
    Remove the last node from lru.
    Insert the new key-value pair at the front.
    Update cacheMap
    */

    // If no space left 
    if(cacheMap.size() == capacity_)
    {
        // get It 
        auto listIt = --(lru.end());
        // --listIt;  // points to the last node [tail] lruList<pair>
        
         // remove pair <key , value>  from cache
        cacheMap.erase(listIt->first);  // returns key 

        // remove pair<key, value> form lruList
        lru.pop_back();  
    }

    // Insert new key-value at the front [ Recency tracking ]
    lru.emplace_front(key , value);  

    // Store the key and its list iterator in the cacheMap
    cacheMap.emplace(key, lru.begin());
}


/**
 * 
 1. Search for the key.
 
 2. If not found, return -1.
 
 3. Retrieve the list iterator.
 
 4. Save the value.
 
 5. Move the node to the front using splice().
 
 6. Return the saved value.
 */

int LRUCache:: get(int key)
{
    // get It
    auto mapIt = cacheMap.find(key);

    // if key not exists in cacheMap 
    if(mapIt == cacheMap.end())
    {   
        cout << "key not found\n";
        return -1;
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