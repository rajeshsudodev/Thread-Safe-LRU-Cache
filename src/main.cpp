
#include <iostream>
// #include "LRUCache.cpp" cmake separately compiles and links
#include "LRUCache.h"
#include <string>
#include <thread>
#include <atomic>
#include <vector>

std:: atomic<int> cacheHit{0};
std:: atomic<int> cacheMiss{0};

using std:: cout , std:: endl;

void Int()
{

    LRUCache<int, int> cache(3);
    cache.put(1, 10);
    cache.put(2, 20);

    if(std:: optional<int>result = cache.get(2); result.has_value())
    {
        cout << result.value() << '\n';
    }
    else cout << "Key not found\n";
}

void Generic()
{
    LRUCache<int , std:: string> cache1(3);
    cache1.put(1, "one");
    cache1.put(2, "two");
    cache1.put(3, "three");

    cout << *cache1.get(1) << '\n';
    cout << *cache1.get(2) << '\n';
    cout << *cache1.get(3) << '\n';


    LRUCache<std:: string , std:: string> cache2(3);

    cache2.put("one", "one");
    cache2.put("two", "two");
    cache2.put("three", "three");
    cache2.put("4", "4");

    cout << *cache2.get("two") << '\n';
    cout << *cache2.get("three") << '\n';
    cout << *cache2.get("4") << '\n';

}

void put(LRUCache<int,int>& cache)

{
    for(int i {0} ;i < 100; ++i)
    {
        cache.put(i, i*10);
    }

}

void get(LRUCache<int, int>& cache)
{

    for(int i {0}; i < 100; ++i)
    {
        if(auto result = cache.get(i); result) cacheHit.fetch_add(1, std:: memory_order_relaxed);
        else cacheMiss.fetch_add(1, std:: memory_order_relaxed);
    }
}

void Test( LRUCache<int, int>& cache)
{

    bool pass = true;
    std:: thread t1([&] { put(cache); });
    std:: thread t2([&] { get(cache); });

    t1.join();
    t2.join();

}

void ConcurrencyTest()
{
    constexpr int threadCount = 10;
    LRUCache<int, int>  cache(100);

    std:: vector<std::thread> threads;
    threads.reserve(threadCount);
   for(int i {0} ; i < threadCount ; ++i)
   {

        threads.emplace_back([&cache, i]
        {
            Test(cache);
        });
    }

    for(auto &thread : threads)
    {
        thread.join();
    }
}

int main()
{
    std::cout << "LRU Cache Project Started!\n";

    std:: thread t1(ConcurrencyTest);
    t1.join();
    cout << "CacheHit : " << cacheHit.load() << '\n';
    cout << "CacheMiss : " << cacheMiss.load() << '\n';
    // Generic();

    return 0;
}
