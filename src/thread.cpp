#pragma once

#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>

static void TestThreadDetachUnsafe(int& x)
{
    //std::thread([&x](){//按引用捕获
    std::thread([x](){//按值捕获，不会导致线程访问到已经销毁的变量，只要捕获的时候，变量x的生命周期还在就没问题。
        std::cout << "thread start" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "x:" << x << std::endl;//这里的x可能已经生命周期结束了
    }).detach();
}

static void TestThreadForgetJoinAndDetach()
{
    std::thread t([](){
        std::cout << "thread start" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "thread end" << std::endl;
    });
    //t.detach();
    //t.join();
    //t.join();
}

static void TestThreadDetachAfterProcessExit()
{
    std::thread t([](){
        for(int i=0;i<5;i++)
        {
            std::cout << "i=" <<i<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    t.detach();
}

static void TestJThread()
{
    std::cout << "TestJThread Calling" << std::endl;
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

void TestThread()
{
    //测试忘记对std::thread调用join或者detach后，程序会异常退出
    //TestThreadForgetJoinAndDetach();

    //测试 当主函数main()返回后主线程就结束了，detach的子线程不能再去访问主线程作用域的相关变量，否则会发生ub未定义行为
    {
        int x =10;
        TestThreadDetachUnsafe(std::ref(x));
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //测试进程退出后，detach的子线程是否仍能继续执行
    TestThreadDetachAfterProcessExit();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //测试jthread析构会自动join以及支持中断
    TestJThread();
}