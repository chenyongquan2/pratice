#include "smart_ptr_mul_thread_safe.h"
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
#include <memory>
#include <thread>
#include <iostream>
#include <mutex>

std::shared_ptr<int> global_ptr = std::make_shared<int>(0);
std::mutex mtx;

void increment() {
    for (int i = 0; i < 1000000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        (*global_ptr)++;
    }
}

void TestSmartPtrMulThreadSafe() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << *global_ptr << std::endl; // 结果可能小于2000000
}

