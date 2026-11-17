
#include <iostream>
// #include "LRUCache.cpp" cmake separately compiles and links
#include "LRUCache.h"
using std:: cout , std:: endl;

int main()
{
    std::cout << "LRU Cache Project Started!\n";

    LRUCache cache(3);
    cache.put(1, 10);
    cache.put(2, 20);

    if(std:: optional<int>result = cache.get(2); result.has_value())
    {
        cout << result.value() << '\n';
    }
    else cout << "Key not found\n";
    
    
    
    return 0;
}
