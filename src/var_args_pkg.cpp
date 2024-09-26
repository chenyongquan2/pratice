#include "var_args_pkg.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <tuple>

template<typename... Args>
void ProcessTupleItem(const std::tuple<Args...>& t)//Args...表示这是一个类型参数包
{
    //std::apply的callable要求参数匹配： 可调用对象的参数数量和类型应该与传入的元组中的元素数量和类型相匹配
    std::apply([](const auto&... args){
        //错误写法：折叠表达式 ... 不能直接用在 << 运算符的中间。std::cout << ... << args 这种语法是不合法的。
        //std::cout << ... << args << std::endl;

        //正确写法形如: (args的相关操作表达式),...
        ((std::cout << args << " "), ...);
        
    }, t);
    std::cout << std::endl;
}

template <typename Callable,typename... Args>
void ProcessTupleItemWithCallable(Callable&& callable, const std::tuple<Args...>& t)
{
    std::apply([callable](const auto&... args){
        //折叠表达式的使用
        (callable(args),...);
    },t

    );
}


/////////////////////////////
//测试VA_ARGS宏写法
template<typename... Args>
void log_helper(std::ostream& os, Args&&... args) {
    (os << ... << std::forward<Args>(args));
}

#define LOG_INFO(...) \
    do { \
        std::cout << __FILE__ << ":" << __LINE__ << ":" << __func__ << " "; \
        log_helper(std::cout, __VA_ARGS__); \
        std::cout << std::endl; \
    } while(0)

void TestMarcoVA_ARGS()
{
    LOG_INFO("hello world","123");
    LOG_INFO("End");
}

/////////////////////////////
//do while(0) 的用法：do-while(0) 是一个不进行循环的"假"循环结构，它的主要目的是提供一个安全、完整的语句块，使宏定义更加健壮和灵活。
#define BAD_MARCO(x) \
    if(x) \
        std::cout << x << std::endl;\
    else\
        std::cout << "No value" << std::endl;

#define GOOD_MARCO(x) \
    do { \
        if(x) \
        std::cout << x << std::endl;\
        else\
            std::cout << "No value" << std::endl;\
    }while(0)



void TestDoWhileZeroFakeLoop()
{
    bool bTest = true;
    if(bTest)
        //BAD_MARCO(bTest);
        GOOD_MARCO(bTest); 
        
    else
        std::cout << "Else" << std::endl;

    //展开后：语法问题：src\var_args_pkg.cpp(67): error C2181: 没有匹配 if 的非法 else
    // if (condition)
    //     if(value) 
    //          std::cout << value << std::endl; 
    //     else std::cout << "No value" << std::endl;
    // else
    //     doSomethingElse();
}

/////////////////////////////

void TestVarArgsPkg() {
    auto t1=std::make_tuple(1,"abc",false,1.44);
    auto t2=std::make_tuple("hello","world",3);

    ProcessTupleItem(t1);
    ProcessTupleItem(t2);

    std::cout << "************************" <<std::endl;

    auto printFunc = [](auto&& t){
        std::cout << t << "\t";
    };

    ProcessTupleItemWithCallable(printFunc, t1);
    std::cout << std::endl;
    ProcessTupleItemWithCallable(printFunc,t2);
    std::cout << std::endl;

    TestMarcoVA_ARGS();

    TestDoWhileZeroFakeLoop();
}
