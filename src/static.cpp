#include "static.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <string>
#include <tuple>
#include <string_view>
#include <utility>
#include <thread>
#include <array>
#include <vector>
#include <mutex>

static int g_static_int = 0;
static std::mutex g_static_int_mutex;

//测试static的线程安全性
void TestStaticThreadSafe()
{
    auto increment = []()
    {
        for(int i=0;i<100000;i++)
        {
            //静态变量线程不安全，因此需要同步手段，例如加锁或者原子变量等手段
            //std::lock_guard<std::mutex> lock_guard(g_static_int_mutex);
            g_static_int++;
        }
    };
    constexpr size_t thread_count = 3;
    std::array<std::thread, thread_count> threads;
    for(size_t i=0;i<thread_count;i++)
    {
        threads.at(i) = std::thread(increment);
    }
    for(size_t i=0;i<thread_count;i++)
    {
        threads.at(i).join();
    }

    //判断结果是否符合预期
    std::cout << "g_static_int="<<g_static_int<<std::endl;//g_static_int=112224，而不是300000，可见static本身不是线程安全的
}

//测试static局部变量的初始化的线程安全性
class Singleton {
public:
    //该构造函数只会被调用一遍。
    Singleton() { std::cout << "Singleton constructed\n"; }
    ~Singleton() {
        std::cout << "Singleton destructed\n";
    }
};

//static Singleton g_Singleton;

static Singleton& getSingleton() 
{
    //在程序启动时，为static Singleton s分配好了内存，并且完成了零初始化
    //在第一次调用到这里完成实际的初始化(调用构造函数)
    static Singleton s;
    return s;
}

void TestInitThreadSafe()
{
    std::cout << "TestInitThreadSafe Starting\n";
    auto threadFunction = []() {
        //std::cout << "Thread " << std::this_thread::get_id() << " accessing Singleton\n";
        Singleton& s = getSingleton();
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(threadFunction);
    }
    for (auto& t : threads) {
        t.join();
    }
}

void TestStatic()
{
    TestStaticThreadSafe();

    TestInitThreadSafe();
}