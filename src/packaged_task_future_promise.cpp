#include "packaged_task_future_promise.h"
#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <mutex>
#include <future>

int calc(int x,int y)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return x+y;
}

void TestPackagedTaskFuturePromise()
{
    //packaged_task 包装的函数什么时候被调用呢
    //1.显示调用
    //taskCalc(10,20);//一个packaged_task不能被多次调用
    {
        std::packaged_task<int(int,int)> taskCalc2(calc);
        taskCalc2(100,200);//主动调用
        std::future<int> calcResult2 = taskCalc2.get_future();
        std::cout << "wait for result2=" << calcResult2.get() << std::endl;
    }
    //2.在现场执行的时候调用
    {
        std::packaged_task<int(int,int)> taskCalc(calc);
        std::future<int> calcResult = taskCalc.get_future();

        //因为std::packaged_task不支持复制，所以这里经过std::move转右值，std::thread构造函数的第一个参数支持接收右值，所以这里可以传入taskCalc
        std::thread t(std::move(taskCalc),10,20);

        std::cout << "wait for result=" << calcResult.get() << std::endl;

        t.join();
    }

    //std::packaged_task 和 std::future 和std::promise不可复制，可移动
    {
        std::packaged_task<int(int,int)> taskCalc3(calc);
        //std::packaged_task<int(int,int)> taskCalc4 = taskCalc3;
        std::packaged_task<int(int,int)> taskCalc4Move = std::move(taskCalc3);
        //这就是为啥线程的第一个参数的可调用对象需要用std::move的原因了
        //std::thread t(std::move(taskCalc),10,20);
    }

    {
        //这里为啥要传引用，因为std::promise和std::future是可移动不可复制的
        auto produce = [](std::promise<int>&p)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            p.set_value(100);
            std::cout << "produce result=100" << std::endl;
        };
        auto consume = [](std::future<int>&f)
        {
            std::cout << "consume result=" << f.get() << std::endl;
        };

        std::promise<int> p;
        std::future<int> f=p.get_future();

        //std::thread的线程执行函数的传参方式默认是传值的，而不支持传引用，所以这里需要借助std::ref的辅助;
        std::thread t1(produce,std::ref(p));
        std::thread t2(consume,std::ref(f));
        
        t1.join();
        t2.join();
    }
}