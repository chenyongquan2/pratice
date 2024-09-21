#include "ref_safe.h"
#include <iostream>
#include <string>

static std::string ReturnValue()
{
    std::string pwd = "12343445465656hghghg55656676jhhjkkioiiiiiiiisdrreerer";
    pwd.append("123456");

    return pwd;
}

static std::string& ReturnLeftValue()
{
    std::string ans = "12343445465656hghghg55656676jhhjkkioiiiiiiiisdrreerer";
    ans.append("123456");

    return ans;
}

static std::string&& ReturnRightValue()
{
    std::string pwd = "12343445465656hghghg55656676jhhjkkioiiiiiiiisdrreerer";
    pwd.append("123456");
    return std::move(pwd);
}

void TestRefSafe()
{
    try
    {
        //Todo:像这种严重的非法内存问题，是无法被异常给捕获的；
        if(0)
        {
            //不ok，因为const左值引用不能具有延长左值的生命周期，这里ReturnLeftValue()返回了左值，但是其所绑定的对象生命周期已经结束了
            const std::string& pwd = ReturnLeftValue();
            std::cout << "pwd:" << pwd << std::endl;
        }

        if(0)
        {
            //不ok，这里ReturnLeftValue()返回了左值，但是其所绑定的对象生命周期已经结束了
            std::string& pwd1 = ReturnLeftValue();
            std::cout << "pwd1:" << pwd1 << std::endl;
        }
       
        if(0)
        {
            //不ok，因为const左值引用的延长生命周期适用于绑定纯右值(prvalue，包含字面量和临时值)
            const std::string& pwd2 = ReturnRightValue();//ok
            std::cout << "pwd2:" << pwd2 << std::endl;
        }

        if(0)
        {
            //不ok，这里右值引用绑定的对象生命周期已经结束了
            std::string&& pwd22 = ReturnRightValue();//ok
            std::cout << "pwd22:" << pwd22 << std::endl;
        }

        // if(1)
        // {
        //     //ok，因为const左值引用的延长生命周期适用于纯右值(ReturnValue返回值是一个临时值)
        //     const std::string& pwd3 = ReturnValue();
        //     std::cout << "pwd3:" << pwd3 << std::endl;
        // }

        if(1)
        {
            //ok，并且因为RVO的机制，这里ReturnValue的返回值的临时值，会直接构造在外部接收的pwd3上。
            //所以也不会有拷贝的性能开销
            std::string pwd4 = ReturnValue();
            std::cout << "pwd4:" << pwd4 << std::endl;
        }

        if(0)
        {
            //Todo:编译器差距问题，我本地的编译器编出来的代码，没有崩，但是别的机器上不同的编译器可能会崩；
            //不ok，右值引用绑定的右值(函数返回值)，已经被释放了
            std::string&& pwd5 = ReturnValue();
            std::cout << "pwd5:" << pwd5 << std::endl;
        }

        if(1)
        {
            //Todo:编译器差距问题，我本地的编译器编出来的代码，没有崩，但是别的机器上不同的编译器可能会崩；
            //不ok，因为const左值引用的延长生命周期不适用于绑定亡值(xvalue)
            //ReturnValue本身返回值是一个临时值，std::move(x)后变成了亡值(xvalue)
            const std::string& pwd6 = std::move(ReturnValue());
            std::cout << "pwd6:" << pwd6 << std::endl;
        }

        if(1)
        {
            //ok，因为const左值引用的延长生命周期适用于纯右值(ReturnValue返回值是一个临时值)
            const std::string& pwd66 = ReturnValue();
            std::cout << "pwd66:" << pwd66 << std::endl;
        }

        // {
        //     SPDLOG_ERROR("Test1 Begin");
        //     const std::string& ref = std::string("343545465565ggytyyty");
        //     SPDLOG_ERROR("Test1 ref: {}", ref);
        //     SPDLOG_ERROR("Test1 End");
        //     std::this_thread::sleep_for(std::chrono::seconds(3));
        //     SPDLOG_ERROR("Test2 Begin");
        //     const std::string& ref2 = std::move(std::string("343545465565ggytyyty"));
        //     SPDLOG_ERROR("Test2 ref2: {}", ref2);
        //     SPDLOG_ERROR("Test2 End");

        // }
    }
    catch (const std::exception& e)
    {
        //把异常信息给打印出来
        std::cout << "exception occured:" << e.what() << std::endl;
    }
    
}


