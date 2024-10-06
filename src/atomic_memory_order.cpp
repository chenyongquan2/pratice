#include "atomic_memory_order.h"

#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>
#include <optional>
#include <functional>
#include <array>
#include <algorithm>
#include <atomic>
#include <thread>
#include <mutex>

//////////////////////////////////
//MemoryFence case:
std::atomic<int> x(0);
std::atomic<int> y(0);
std::atomic<int> z(0);

void ThreadA_write_x_then_y()
{
    //Todo:假如这里没有内存屏障，那么由于指令重排，x和y的写入顺序可能被重新排序，导致y的写入先于x的写入发生。
    x.store(1,std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);  // 写屏障,确保”屏障上面的写操作"不会被安排到”屏障下面的写操作" 之后 去执行，保持这两个模块的相对顺序
    y.store(1, std::memory_order_relaxed);  // 写入y
}

void ThreadB_read_y_then_x()
{
    //Todo:那么这个线程这里就可能能看到y==1但是x==0的情况
    while (y.load(std::memory_order_relaxed) == 0) {}  // 等待y变为1
    std::atomic_thread_fence(std::memory_order_acquire);  // 读屏障, 确保”屏障上面的读操作"不会被安排到”屏障下面的读操作" 之后 去执行，保持这两个模块的相对顺序
    if (x.load(std::memory_order_relaxed) == 0)  // 读取x
        ++z;
}

void TestMemoryFence()
{
    std::thread a(ThreadA_write_x_then_y);
    std::thread b(ThreadB_read_y_then_x);
    a.join();
    b.join();
    //我们期望的是线程A和线程B保持同步(想按照我们期望的顺序来执行)
    _ASSERT(z.load() == 0);
    std::cout << "x= " << x << ",y=" << y << ",z = " << z << std::endl;
}

//////////////////////////////////////////////
//MemoryOrder和内存屏障的关系
std::atomic<bool> ready(false);
int data = 0;

void producer() {
    // 引入一些计算来增加重排的可能性
    for (int i = 0; i < 1000; ++i) {
        data += i;
    }
    data = 42;  // 非原子写操作
    //Todo:下面这两种写法是等价的
    //写法1:直接在 store 操作上使用 memory_order_release
    ready.store(true, std::memory_order_release);  // 原子写操作
    //写法2:使用一个独立的释放屏障
    // std::atomic_thread_fence(std::memory_order_release);  // 释放屏障
    // ready.store(true, std::memory_order_relaxed);  // 原子写操作
}

void consumer() {
    //Todo:下面这两种写法是等价的
    //写法1：直接在 load 操作上使用 memory_order_acquire
    while (!ready.load(std::memory_order_acquire)) {
        // 引入一些操作来增加重排的可能性
        std::this_thread::yield();
    } 
    //写法2：内存屏障的写法
    // while (!ready.load(std::memory_order_relaxed)) {
    //     // 引入一些操作来增加重排的可能性
    //     std::this_thread::yield();
    // }  // 原子读操作
    // std::atomic_thread_fence(std::memory_order_acquire); // 获取屏障
    _ASSERT(data == 42);
    std::cout << "data: " << data << std::endl;  // 非原子读操作
}

void TestMemoryOrder() {
    std::vector<std::thread> threads;
    constexpr int num_threads = 1;
    //constexpr int num_threads = 100000;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(producer);
        threads.emplace_back(consumer);
    }
    for (auto& t : threads) {
        t.join();
    }
}

//////////////////////////////////////////////
//不同memory_order的作用的例子
std::atomic<int> xx(0), yy(0);
int r1, r2;

void thread1() {
    r1 = yy.load(std::memory_order_relaxed);
    xx.store(r1, std::memory_order_release);
}

void thread2() {
    r2 = xx.load(std::memory_order_acquire);
    yy.store(42, std::memory_order_relaxed);
}

void TestDiffientMemoryOrder() {
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.join(); 
    t2.join();
    std::cout << "r1: " << r1 << ", r2: " << r2 << std::endl;
}

//////////////////////////////////////////////
//测试std::memory_order_acq_rel用于读-修改-写（Read-Modify-Write，RMW）操作
std::atomic<int> counter(0);
std::atomic<bool> readyForTestMemoryOrderAcqRelForReadModifyWriteBehavior(false);
std::vector<int> results(10, 0);

void worker(int id) {
    // 等待主线程发出开始信号
    while (!readyForTestMemoryOrderAcqRelForReadModifyWriteBehavior.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    // 执行读-修改-写操作
    int previous = counter.fetch_add(1,std::memory_order_acq_rel);
    //int previous = counter.fetch_add(1, std::memory_order_acq_rel);
    
    // 存储结果
    results[id] = previous;
}

void TestMemoryOrderAcqRelForReadModifyWriteBehavior() {
    std::cout << "TestMemoryOrderAcqRelForReadModifyWriteBehavior" <<std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, i);
    }

    // 发出开始信号
    readyForTestMemoryOrderAcqRelForReadModifyWriteBehavior.store(true, std::memory_order_release);

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    // 打印结果
    for (int i = 0; i < 10; ++i) {
        std::cout << "Thread " << i << " saw counter value: " << results[i] << std::endl;
    }
    std::cout << "Final counter value: " << counter.load(std::memory_order_relaxed) << std::endl;
}

//////////////////////////////////////////////
//测试原子操作的API
//测试 对比 compare_exchange_strong 和 compare_exchange_weak
void TestCompareAndSwapStrongAndWeak()
{
    std::atomic<int> value(5);

    auto demonstrate_compare_exchange_strong = [&value]() {
        int expected = 5;
        bool success = value.compare_exchange_strong(expected, 10);
        std::cout << "Strong CAS " << (success ? "succeeded" : "failed") 
                << ". Value is now " << value.load() << std::endl;
    };

    auto demonstrate_compare_exchange_weak = [&value]()
    {
        int expected = 5;
        while (!value.compare_exchange_weak(expected, 10)) {
            // 如果失败，expected 会被更新为当前值
            std::cout << "Weak CAS failed. Expected is now " << expected << std::endl;
        }
        std::cout << "Weak CAS finally succeeded. Value is now " << value.load() << std::endl;
    };

    demonstrate_compare_exchange_strong();    
    value.store(5);  // 重置值
    demonstrate_compare_exchange_weak();

}

void TestAtomicAPI() {
    std::atomic<int> counter(0);
    auto incrementFunc =[&counter]() {
        for (int i = 0; i < 1000; ++i) {
            counter.fetch_add(1,std::memory_order_relaxed);
        }
    };

    std::thread t1(incrementFunc);
    std::thread t2(incrementFunc);

    t1.join();
    t2.join();

    std::cout << "Final counter value: " << counter.load(std::memory_order_relaxed) << std::endl;

    // 使用 exchange 操作
    int old_value = counter.exchange(100,std::memory_order_acq_rel);
    std::cout << "Old value: " << old_value << ", New value: " << counter.load() << std::endl;

    // 使用 compare_exchange_strong 操作
    int expected = 100;
    bool success = counter.compare_exchange_strong(expected,200,std::memory_order_acq_rel);
    std::cout << "CAS success: " << success << ", Current value: " << counter.load() << std::endl;

    //对比 compare_exchange_strong 和 compare_exchange_weak
    TestCompareAndSwapStrongAndWeak();
}

/////////////////////////////////
//总入口
void TestAtomicMemoryOrder()
{
    std::cout << "TestAtomicMemoryOrder" << std::endl;
    // TestMemoryFence();

    // TestMemoryOrder();

    TestDiffientMemoryOrder();

    TestMemoryOrderAcqRelForReadModifyWriteBehavior();

    TestAtomicAPI();
}