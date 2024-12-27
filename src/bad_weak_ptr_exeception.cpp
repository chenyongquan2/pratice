#include "weak_ptr.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <thread>
#include <vector>

class PrivateImpl:std::enable_shared_from_this<PrivateImpl>
{
public:
    void TestSharedFromThis()
    {
        try
        {
            auto p = shared_from_this();
        }
        catch (const std::exception& e)
        {
            //exeception occured:bad_weak_ptr
            std::cout << "exeception occured:" << e.what() << std::endl;
        }
        
    }
};

void TestBadWeakPtrExeception() 
{
    auto p = std::make_shared<PrivateImpl>();
    p->TestSharedFromThis();

}
