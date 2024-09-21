#include "callable.h"
#include <cstring>
#include <iostream>
#include <functional>

//Todo:顺便作为可变参数的使用例子
template<typename Callable,typename... Args>
//void Call(Callable callable,Args... args)
//Args...: 这是一个参数包，表示零个或多个类型参数
void CallWithoutFastward(Callable&& callable,Args&&... args)//T&&: 这是一个通用引用，可以绑定到左值或右值。
{
    //args... 是参数包的展开

    // args 这个变量在 Call 函数内部确实是一个左值。它可以接收左值或右值，这是因为它被声明为通用引用（Args&&...），
    // 但一旦进入函数体，它就成为了一个有名字的实体，因此在函数内部被视为左值。
    // 这就是为什么在不使用 std::forward 的情况下，我们无法保持传入参数的原始值类别。
    // args 可以接收任何值类别(可绑定左值右值等)的参数，但在函数内部，它总是一个左值。
    // 这也解释了为什么在处理通用引用时，使用 std::forward 来保持原始值类别如此重要。

    callable(args...);

    //Todo:完美转发例子，确保参数的值类别(所谓的值类别指左值，右值和泛左值)在传递过程中得到保留。
    //callable(std::forward<Args>(args)...);
}

template<typename Callable,typename... Args>
//void Call(Callable callable,Args... args)
//Args...: 这是一个参数包，表示零个或多个类型参数
void CallWithFastward(Callable&& callable,Args&&... args)//T&&: 这是一个通用引用，可以绑定到左值或右值。
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


/////////////////////////////////////////////////////////
//测试完美转发的例子
void printString(std::string& s) { std::cout << "lvalue: " << s << std::endl; }
void printString(std::string&& s) { std::cout << "rvalue: " << s << std::endl; }

void TestForward()
{
    std::string str = "Hello";
    //Todo: 错误写法，因为有两个同名的函数，编译器无法确定你想要绑定哪个版本的 printString 函数。
    // Call(printString, str);  // 输出: lvalue: Hello
    // Call(printString, std::move(str));  // 输出: rvalue: Hello

    //解决方法: 明确指定你想要绑定的 Send 函数版本。你可以使用函数指针来消除歧义，例如
    void (*printStringLeftValArgVersion)(std::string&) = printString;
    void (*printStringRightValArgVersion)(std::string&&) = printString;
    
    
    {
        std::cout << "[Begin] CallWithoutFastward" << std::endl;
        CallWithoutFastward(printStringLeftValArgVersion, str);  // 输出: lvalue: Hello
        //当Call内部没有完美转发时，
        //会编译报错，因为 在Call函数内部，这个右值被绑定到一个命名的函数参数上，这个过程使它变成了一个左值。
        //当尝试将这个现在已经是左值的参数传递给期望接收右值引用(std::string&&)的printString函数重载版本时，编译器发现无法将左值绑定到右值引用上。
        //编译不用通过
        //CallWithoutFastward(printStringRightValArgVersion, std::move(str));

        //编译通过，但是输出// 输出: lvalue: Hello 而不是预期的 rvalue: Hello
        CallWithoutFastward(printStringLeftValArgVersion,std::move(str));
        std::cout << "[End] CallWithoutFastward" << std::endl;
    }

    {
        std::cout << "[Begin] CallWithFastward" << std::endl;
        CallWithoutFastward(printStringLeftValArgVersion, str);  // 输出: lvalue: Hello
        //excepted:  rvalue: Hello
        CallWithFastward(printStringRightValArgVersion, std::move(str));
        std::cout << "[End] CallWithFastward" << std::endl;
    }
}

