#include "marco_inline_constexpr.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <tuple>
#include <string_view>
#include <utility>

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
//replace marco by using inline function or constexpr function
// - 理论上，内联函数的定义可以放在 .cpp 文件中。
// - 但是，为了让内联函数正常工作，编译器需要在每个调用点看到函数的完整定义。
// - 如果定义在 .cpp 文件中，其他编译单元将无法看到这个定义，从而无法进行内联。
// - 因此，通常建议将内联函数的定义放在头文件中，以确保所有调用点都能看到完整定义。
template<typename T>
inline void ReplaceMarcoByUsingInlineFunction(T&& t)
{
    if(t)
        std::cout << t << std::endl;
    else
        std::cout << "No value" << std::endl;
}

//constexpr 函数提供了编译时求值的可能性，但不保证一定会发生。它的实际行为取决于使用上下文和编译器的决定
//下面的ReplaceMarcoByUsingConstexprFunction会有副作用，因此这里不会发生编译器求值
//原始函数由于包含 I/O 操作和副作用，不会进行编译期求值。要利用 constexpr 的特性，需要将纯粹的计算逻辑与 I/O 操作分离
// template<typename T>
// constexpr void ReplaceMarcoByUsingConstexprFunctionWitoutComiplePerroidCalc(T&& t)
// {
//     if(t)
//         std::cout << t << std::endl;
//     else
//         std::cout << "No value" << std::endl;
// }

//正确写法
// template<typename T>
// constexpr std::string_view get_value_or_no_value(T&& x) {
//     return x ? std::string_view("value") : std::string_view("No value");
// }
template<typename T>
constexpr const char* get_message(const T& t) {
    return t ? "Has value" : "No value";
}


template<typename T>
inline void ReplaceMarcoByUsingConstexprFunctionWitoutComiplePerroidCalc(const T& t) {
    auto message = get_message(t);
    std::cout << message << std::endl;
    if (t) {
        std::cout << t << std::endl;
    }
}

void TestReplaceMarco()
{
    {
        bool bTest = true;
        if(bTest)
            ReplaceMarcoByUsingInlineFunction(bTest); 
        else
            std::cout << "Else" << std::endl;
    }
    
    {
        bool bTest = true;
        if(bTest)
            ReplaceMarcoByUsingConstexprFunctionWitoutComiplePerroidCalc(bTest); 
        else
            std::cout << "Else" << std::endl;
    }
    
}

/////////////////////////////
void TestMarcoInlineConstexpr() {
    TestDoWhileZeroFakeLoop();
    std::cout <<"TestDoWhileZeroFakeLoop====="<<std::endl;
    TestReplaceMarco();
    std::cout <<"TestReplaceMarco========"<<std::endl;
}
