#include "bind.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <utility>

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


struct TaskProcessor
{
public:
    bool ProcessTask(const std::string& reqId, const std::string& name)
    {
        std::cout << "ProcessTask reqId:" <<reqId << ",name:" << name << std::endl;
        return true;
    }
};


void Response(const std::string& reqId, const std::string& msg)
{
    std::cout << "Response reqId:" << reqId << ",msg:" << msg << std::endl;
}

//基于模板实现一个线程池的雏形
struct ThreadPool
{
    template<typename Callable>
    void AddTask(Callable&& callable)
    {
        //启动一个线程去执行
        std::jthread task(callable);
        task.join();
    }

    template<typename Callable>
    void AddTaskWithLambda(Callable&& callable, const std::string& sessionId)
    {
        //启动一个线程去执行
        std::jthread task([sessionId, callable](){
            std::cout << "sessionId:" << sessionId << ",thread id:" << std::this_thread::get_id() << std::endl;
            callable();
            //std::forward(callable());
        });
        task.join();
    }
};

void TestBind()
{
    TaskProcessor t;
    std::jthread task(std::bind(&TaskProcessor::ProcessTask,&t,"req1","param"));
    task.join();

    std::jthread task2(std::bind(Response,"req1","hello"));
    task2.join();

    std::cout << "main thread id:" << std::this_thread::get_id() << std::endl;

    ThreadPool pool;
    pool.AddTask(std::bind(&TaskProcessor::ProcessTask,&t,"req1","param"));
    pool.AddTaskWithLambda(std::bind(Response,"req1","hello"),"session1");


}
