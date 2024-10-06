#include "thread_local.h"
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
#include <sstream>
#include <mutex>

thread_local static int g_thread_local_int = 0;
static std::mutex g_cout_mutex;

void increment_print_counter(int id)
{
    ++g_thread_local_int;
    //先准备好输出的内容字符串
    std::ostringstream ss;
    ss << "Thread " << id << " g_thread_local_int = " << g_thread_local_int << std::endl;
    //Todo:std::cout不是线程安全的,这里防止打印的内容错乱，需要加锁
    //std::cout << "Thread " << id << " g_thread_local_int = " << g_thread_local_int << std::endl;
    std::lock_guard<std::mutex> lock_guard(g_cout_mutex);
    std::cout << ss.str();
}

void TestThreadLocal()
{
    auto workerThread = [](int id)
    {
        for(int i=0;i<3;i++)
        {
            increment_print_counter(id);
        }
    };
    constexpr size_t thread_count = 3;
    std::array<std::thread, thread_count> threads;
    for(size_t i=0;i<thread_count;i++)
    {
        threads.at(i) = std::thread(workerThread,i);
    }
    for(size_t i=0;i<thread_count;i++)
    {
        threads.at(i).join();
    }
    //主线程也把thread_local变量打印出来
    std::cout << "Main thread g_thread_local_int = " << g_thread_local_int << std::endl;
}