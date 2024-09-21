#include "callable.h"
#include <cstring>
#include <iostream>
#include <functional>

//Todo:顺便作为可变参数的使用例子
template<typename Callable,typename... Args>
//void Call(Callable callable,Args... args)
//Args...: 这是一个参数包，表示零个或多个类型参数
void Call(Callable&& callable,Args&&... args)//T&&: 这是一个通用引用，可以绑定到左值或右值。
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

struct Connection
{
    Connection()
    {
        std::cout << "Connection constructed" << std::endl;
    }
    ~Connection()
    {
        std::cout << "Connection destructed" << std::endl;
    }
    void Send(const std::string& msg)
    {
        std::cout << "Send msg:" << msg << std::endl;
    }
    void Send(const std::string& requestId, const std::string& msg)
    {
        std::cout << "Send requestId:" << requestId << "msg:" << msg << std::endl;
    }
};


void TestCallable() {
    std::string msg("hello");
    std::string req("req1");
    Connection conn;
    //Todo：当你使用 std::bind 时，编译器无法自动确定你想要绑定哪个版本的 Send 函数，因为它们的名称相同。这种情况下，编译器会报告无法推导出正确的函数重载。
    //auto sendCallable1 = std::bind(&Connection::Send, &conn, std::placeholders::_1);

    //解决方法: 明确指定你想要绑定的 Send 函数版本。你可以使用函数指针来消除歧义，例如
    using FuncSendOneArgType = void(Connection::*)(const std::string&);
    //void (Connection::*pSendOneArg)(const std::string&) = &Connection::Send;
    FuncSendOneArgType pSendOneArg = &Connection::Send;
    auto sendCallable1 = std::bind(pSendOneArg,&conn, msg);
    Call(sendCallable1);

    //void (Connection::*pSendTwoArgFake)(const std::string& ) = &Connection::Send;
    FuncSendOneArgType pSendTwoArgFake = &Connection::Send;
    auto sendCallable2 = std::bind(pSendTwoArgFake,&conn,req);
    //Todo:下面的多余用不上的可变参数会直接被丢弃掉
    Call(sendCallable2,msg);

    
    void (Connection::*pSendTwoArg)(const std::string&, const std::string&) = &Connection::Send;
    auto sendCallable3 = std::bind(pSendTwoArg,&conn,req,std::placeholders::_1);
    //Todo:下面的多余用不上的可变参数会直接被丢弃掉
    Call(sendCallable3,msg);


    std::cout << "Program completed" << std::endl;
}
