#pragma once

#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>

#include "singleton_impl.h"

void TestJThreadStop()
{
    //std::jthread底层 是如何巧妙地处理了是否传递 stop_token 的问题的呢？
    //构造函数内部，使用了 if constexpr 来在编译时决定如何调用可调用对象：
    //使用 is_invocable_v (类型特征里的一个模板函数)来检查 _Fn 是否可以接受 stop_token 作为第一个参数。如果可以，就传递 stop_token；如果不可以，就直接传递其他参数。
    auto download = [](std::stop_token st){
        for(int i = 0; i < 20; i++)
        {
            // 模拟下载块
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            // 检查是否请求停止
            if (st.stop_requested()) {
                std::cout << "Download interrupted!\n";
                return;
            }
            
            std::cout << "Downloaded block " << (i + 1) << "\n";
        }
    };

    std::jthread downloadThread(download);

    // 模拟用户请求停止下载
    std::this_thread::sleep_for(std::chrono::seconds(5));
    downloadThread.request_stop(); // 请求停止下载

    // std::jthread 析构后，会自动调用 join()

}

void TestJThread()
{
    TestJThreadStop();
}