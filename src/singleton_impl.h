#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <memory>

//方式1：基于std::call_once来实现单例模式(懒汉式)
class SingletonImplWithOnceCall
{
public:
    //静态成员函数
    //返回值的两种方式:
    //方式1：返回裸指针出去
    // static SingletonImplWithOnceCall* GetInstance() 
    // {
    //     //lambda无需捕获this指针，因为静态成员函数不依赖于this指针
    //     std::call_once(m_onceFlag, [](){
    //         //方式1：裸指针
    //         m_pSingletonImpl = new SingletonImplWithOnceCall();
            
    //     });
    //     return m_pSingletonImpl;
    // }

    //方式2: 返回对象的引用出去
    static SingletonImplWithOnceCall& GetInstance()
    {
        //lambda无需捕获this指针，因为静态成员函数不依赖于this指针
        std::call_once(m_onceFlag, [](){
            //方式2: 智能指针
            //下面的代码编译不过，因为std::make_unique函数模板，相当于当前类SingletonImplWithOnceCall来说，是一个外部函数;
            // std::make_unique<SingletonImplWithOnceCall>() 内部会去调用 SingletonImplWithOnceCall的私有的构造函数
            // 但是其属于外部函数，没有访问权限。
            //m_spSingletonImpl = std::make_unique<SingletonImplWithOnceCall>();

            //为什么这里的代码却可以编译得过呢？
            //因为 c++标准规定，在类的成员函数（包括静态成员函数）内部定义的lambda表达式被视为该类的友元函数。
            m_spSingletonImpl.reset(new SingletonImplWithOnceCall());
        });
        return *m_spSingletonImpl;
    }

    ~SingletonImplWithOnceCall()
    {
        std::cout << "~SingletonImplWithOnceCall() calling" << std::endl;
    }

    //把拷贝构造函数和拷贝赋值函数给delete掉
    SingletonImplWithOnceCall(const SingletonImplWithOnceCall&) = delete;
    SingletonImplWithOnceCall& operator=(const SingletonImplWithOnceCall&) = delete;


    void PrintObjectAddr()
    {
        std::cout << "Addr:" <<this << std::endl;
    }

private:
    //构造函数得私有
    SingletonImplWithOnceCall()
    {
        std::cout << "SingletonImplWithOnceCall() calling" << std::endl;
    }

private:
    //静态成员变量
    //方式1：裸指针
    // 选用裸指针管理的内存，没看到手动delete，会不会有内存泄漏的风险呢？
    // 不会，因为单例对象的生命周期通常设计为整个程序运行期间都存在；
    // 程序结束了后，虽然我们没有手动delete来手动释放内存，但是操作系统会在程序结束后回收所有内存；
    // 如果特别在意这个，可以使用std::unique_ptr来交由智能指针来负责释放所管理的内存；
    //static SingletonImplWithOnceCall* m_pSingletonImpl;
    
    //方式2：智能指针
    static std::unique_ptr<SingletonImplWithOnceCall> m_spSingletonImpl;
    static std::once_flag m_onceFlag;
};


//Todo: 
//#pragma once 确实只能保证该头文件在单个编译单元（即一个 .cpp 文件及其包含的所有头文件）中只被包含一次。
//它不能防止多个不同的 .cpp 文件各自包含这个头文件。
//每个包含该头文件的 .cpp 文件都会生成一个独立的编译单元，其中都会包含头文件中定义的静态成员变量。
//这就是为什么即使使用了 #pragma once，静态成员变量的定义仍然可能在多个编译单元中重复出现，最终导致链接错误。

//类的静态成员变量得初始化
// 方式1：裸指针的初始化
//SingletonImplWithOnceCall* SingletonImplWithOnceCall::m_pSingletonImpl = nullptr; 
// 方式2：智能指针的初始化
// std::unique_ptr<SingletonImplWithOnceCall> m_spSingletonImpl;

// std::once_flag SingletonImplWithOnceCall::m_onceFlag;

//声明函数
// void TestSingletonImpl();

//为了防止下面的函数的定义被链接报错，解决方案有两种:
//1是将定义放在cpp,这里是放函数声明
//2是将函数声明为内联函数
inline void TestSingletonImplWithOnceCall()
{
    std::jthread t1([](){
        auto& obj = SingletonImplWithOnceCall::GetInstance();
        obj.PrintObjectAddr();
    });

    std::jthread t2([](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        SingletonImplWithOnceCall::GetInstance().PrintObjectAddr();
    });

    std::jthread t3([](){
        SingletonImplWithOnceCall::GetInstance().PrintObjectAddr();
    });

    //std::jthread会在析构函数时判断是否joinable(),若是，则会去调用.join()等待线程正常结束。
    // t1.join();
    // t2.join();
    // t3.join();
}

/////////////////////////////////////////////////////////////
//方式2：基于局部静态变量来实现单例模式(懒汉式)
class SingletonImplWithStaticLocalVal
{
public:
    static SingletonImplWithStaticLocalVal& GetInstance() 
    {
        static SingletonImplWithStaticLocalVal instance;
        return instance;
    }

    ~SingletonImplWithStaticLocalVal()
    {
        std::cout << "~SingletonImplWithStaticLocalVal() destructor calling" << std::endl;
    }

    SingletonImplWithStaticLocalVal(const SingletonImplWithStaticLocalVal&) = default;
    SingletonImplWithStaticLocalVal& operator=(const SingletonImplWithStaticLocalVal&) = delete;

    void PrintObjectAddr()
    {
        std::cout << "Addr:" <<this << std::endl;
    }
private:
    SingletonImplWithStaticLocalVal()
    {
        std::cout << "SingletonImplWithStaticLocalVal Constructor" << std::endl;
    }

private:
    
};

inline void TestSingletonImplWithStaticLocalVal()
{
    std::jthread t1([](){
        auto& obj = SingletonImplWithStaticLocalVal::GetInstance();
        obj.PrintObjectAddr();
    });

    std::jthread t2([](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        SingletonImplWithStaticLocalVal::GetInstance().PrintObjectAddr();
    });

    std::jthread t3([](){
        SingletonImplWithStaticLocalVal::GetInstance().PrintObjectAddr();
    });

    //std::jthread会在析构函数时判断是否joinable(),若是，则会去调用.join()等待线程正常结束。
    // t1.join();
    // t2.join();
    // t3.join();
}


/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
//方式3：饿汉式
class SingletonImplWithHungryMode
{
public:
    static SingletonImplWithHungryMode& GetInstance() 
    {
        return *m_pSingletonImplWithHungryMode;
    }

    ~SingletonImplWithHungryMode()
    {
        std::cout << "~SingletonImplWithHungryMode() destructor calling" << std::endl;
    }

    SingletonImplWithHungryMode(const SingletonImplWithHungryMode&) = default;
    SingletonImplWithHungryMode& operator=(const SingletonImplWithHungryMode&) = delete;

    void PrintObjectAddr()
    {
        std::cout << "Addr:" <<this << std::endl;
    }
private:
    SingletonImplWithHungryMode()
    {
        std::cout << "SingletonImplWithHungryMode Constructor" << std::endl;
    }

private:
    static SingletonImplWithHungryMode* m_pSingletonImplWithHungryMode;
};

inline void TestSingletonImplWithHungryMode()
{
    std::jthread t1([](){
        auto& obj = SingletonImplWithHungryMode::GetInstance();
        obj.PrintObjectAddr();
    });

    std::jthread t2([](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        SingletonImplWithHungryMode::GetInstance().PrintObjectAddr();
    });

    std::jthread t3([](){
        SingletonImplWithHungryMode::GetInstance().PrintObjectAddr();
    });

    //std::jthread会在析构函数时判断是否joinable(),若是，则会去调用.join()等待线程正常结束。
    // t1.join();
    // t2.join();
    // t3.join();
}


/////////////////////////////////////////////////////////////


//Todo:内敛函数定义一般都放在头文件里，像下面这样只声明的话，会导致链接错误
//inline void TestSingletonImpl();

//将内联函数定义放在头文件中是C++编程的最佳实践，能够充分利用内联函数的特性和优势。
inline void TestSingletonImpl()
{
    TestSingletonImplWithOnceCall();
    std::cout << "Test TestSingletonImplWithOnceCall() finished" <<std::endl;

    TestSingletonImplWithStaticLocalVal();
    std::cout << "Test TestSingletonImplWithOnceCall() finished" <<std::endl;

    TestSingletonImplWithHungryMode();
    std::cout << "Test TestSingletonImplWithHungryMode() finished" <<std::endl;

}


