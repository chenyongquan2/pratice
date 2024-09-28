#include "constexpr.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <tuple>
#include <string_view>
#include <utility>

consteval auto sum(int a, int b)
{
    return a+b;
}

//非类型模板参数要求必须是一个常量表达式
template<typename T,int Size>
struct Buffer
{
    T arr[Size];
};

enum class Priority
{
    Low = 0,
    Medium,
    High,
    Urgent
};

enum PriorityOld
{
    Low = 0,
    Medium,
    High,
    Urgent
};

void TestConstexpr()
{
    constexpr int a=10,b=20;
    int c = sum(a,b);
    std::cout << "c:" << c << std::endl;

    //无法编译期求值的例子：
    //src\constexpr.cpp(21): error C7595: “sum”: 对即时函数的调用不是常量表达式
    // int aa=10,bb=20;
    // int cc = sum(aa,bb);
    // std::cout << "cc:" << cc << std::endl;

    Buffer<int,10> buf;
    Buffer<std::string, 5> buf2;
    Buffer<std::string, sum(10,20)> buf3;
    //编译报错：非类型模板参数要求必须是一个常量表达式
    // int nSize=20;//非常量表达式
    // Buffer<std::string, nSize> buf4;

    //Todo:enum class不能隐式类型转换为int
    Buffer<std::string, static_cast<int>(Priority::High)> buf5;
    Buffer<std::string, PriorityOld::Low> buf6;
}