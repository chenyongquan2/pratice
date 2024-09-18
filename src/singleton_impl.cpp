#include "singleton_impl.h"

//Todo: 
//#pragma once 确实只能保证该头文件在单个编译单元（即一个 .cpp 文件及其包含的所有头文件）中只被包含一次。
//它不能防止多个不同的 .cpp 文件各自包含这个头文件。
//每个包含该头文件的 .cpp 文件都会生成一个独立的编译单元，其中都会包含头文件中定义的静态成员变量。
//这就是为什么即使使用了 #pragma once，静态成员变量的定义仍然可能在多个编译单元中重复出现，最终导致链接错误。

//类的静态成员变量得初始化
// 方式1：裸指针的初始化
//SingletonImplWithOnceCall* SingletonImplWithOnceCall::m_pSingletonImpl = nullptr; 
// 方式2：智能指针的初始化
std::unique_ptr<SingletonImplWithOnceCall> SingletonImplWithOnceCall::m_spSingletonImpl;

std::once_flag SingletonImplWithOnceCall::m_onceFlag;


// static void TestSingletonImplWithOnceCall()
// {
//     std::jthread t1([](){
//         auto& obj = SingletonImplWithOnceCall::GetInstance();
//         obj.PrintObjectAddr();
//     });

//     std::jthread t2([](){
//         std::this_thread::sleep_for(std::chrono::seconds(3));
//         SingletonImplWithOnceCall::GetInstance().PrintObjectAddr();
//     });

//     std::jthread t3([](){
//         SingletonImplWithOnceCall::GetInstance().PrintObjectAddr();
//     });

//     //std::jthread会在析构函数时判断是否joinable(),若是，则会去调用.join()等待线程正常结束。
//     // t1.join();
//     // t2.join();
//     // t3.join();
// }

// void TestSingletonImpl()
// {
//     TestSingletonImplWithOnceCall();
//     std::cout << "TestSingletonImpl() finished" <<std::endl;
// }



//////////////////////////
//方式3：饿汉式
SingletonImplWithHungryMode* SingletonImplWithHungryMode::m_pSingletonImplWithHungryMode = new SingletonImplWithHungryMode();