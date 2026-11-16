#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list> // Doubly Linked List 
#include <unordered_map> // HashMap
#include <utility>   // std:: pair


// LRU Cache = Fixed Capacity + Recency Tracking + Eviction
class LRUCache
{
private:
 
    int capacity_; 

    // Stores key-value pairs.
    std:: list< std:: pair<int , int>> lru;

    // Maps each key to its corresponding list node 
    std:: unordered_map<int , std:: list< std:: pair<int , int>>:: iterator> cacheMap;
public:
    
    explicit LRUCache(int capacity); // Explicit -> only direct initialization allowed

    int get (int key);
    void put (int key , int value);

};




#endif