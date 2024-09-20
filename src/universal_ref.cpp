#include "universal_ref.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <string>
#include <utility>
#include <vector>

//Todo:顺便作为可变参数的使用例子
template<typename Callable,typename... Args>
//void Call(Callable callable,Args... args)
//Args...: 这是一个参数包，表示零个或多个类型参数

//T&&: 这是一个通用引用，可以绑定到左值或右值。
auto CallWithFastward(Callable&& callable,Args&&... args) ->decltype(callable(std::forward<Args>(args)...))
{
    //args... 是参数包的展开

    // args 这个变量在 Call 函数内部确实是一个左值。它可以接收左值或右值，这是因为它被声明为通用引用（Args&&...），
    // 但一旦进入函数体，它就成为了一个有名字的实体，因此在函数内部被视为左值。
    // 这就是为什么在不使用 std::forward 的情况下，我们无法保持传入参数的原始值类别。
    // args 可以接收任何值类别(可绑定左值右值等)的参数，但在函数内部，它总是一个左值。
    // 这也解释了为什么在处理通用引用时，使用 std::forward 来保持原始值类别如此重要。

    //callable(args...);

    //Todo:完美转发例子，确保参数的值类别(所谓的值类别指左值，右值和泛左值)在传递过程中得到保留。
    callable(std::forward<Args>(args)...);
}

//非模板函数中使用 auto&&来表示参数的类型，让编译器来推断参数类型
void getName(auto&& name)
{
    std::cout << "name:" << name << std::endl;
}

std::pair<std::string,int> getDeposit()
{
    return {std::string("cc"),1000000};
}

void TestUniversalRef()
{
    //auto&&
    std::vector<int> vec{1,2,3,4,5};
    for(auto&& i:vec)
    {
        std::cout << i << std::endl; 
    }

    //receieve
    auto&& [name,deposit] = getDeposit();


    //lambda 表达式：让编译器来推断参数类型，然后使用decltype来推断返回值类型
    auto forwarder = [](auto&& name){
        getName(std::forward<decltype(name)>(name));
    };

    forwarder(123);
    forwarder(std::string("cc"));
}

