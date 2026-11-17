
#include <iostream>
// #include "LRUCache.cpp" cmake separately compiles and links
#include "LRUCache.h"
#include <string>
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
int main()
{
    std::cout << "LRU Cache Project Started!\n";

    
    Generic();
    
    return 0;
}
