#include "ref_wrapper.h"
#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <mutex>

template<typename T>
void Add(T param)
{
    ++param;
}

void Increment(int& value) {
    ++value;
}

class SharedResource {
    std::vector<int> data;
    std::mutex mtx;

public:
    void add(int value) {
        std::lock_guard<std::mutex> lock(mtx);
        data.push_back(value);
    }

    void print() {
        std::lock_guard<std::mutex> lock(mtx);
        for (int v : data) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
};

void worker(SharedResource& resource, int value) {
    resource.add(value);
}

//多个线程需要访问一个共享对象SharedResource的测试例子
int TestSharedResource() {
    SharedResource resource;
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        //写法1：std::ref通过refrence_wrapper来传递引用
        //threads.emplace_back(worker, std::ref(resource), i);
        //写法2：lambda表达式&捕获共享对象
        threads.emplace_back([&resource,i](){
            resource.add(i);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    resource.print();
    return 0;
}

void TestRefWrapper()
{
    //用法1:在模板函数中，编译器通常会将参数推导为值类型，而不是引用类型
    int x = 10;
    Add(x);
    std::cout << "Add not throught std::ref, x = " << x << std::endl;  // 输出 10

    Add(std::ref(x));
    std::cout << "Add throught std::ref, x = " << x << std::endl;  // 输出 11

    //用法2：std::thread 构造函数 通过std::ref来传递引用
    //借助std::ref来生成refernce_wrapper,std::thread以值形式接收此refernce_wrapper，然后通过refernce_wrapper的operator&()来达到隐式传递引用的效果
    // std::thread t1(Increment,x);//编译报错
    // t1.join();
    // std::cout << "After t1 thread, x = " << x << std::endl;  // 输出 11

    std::thread t2(Increment, std::ref(x));//借助std::ref来生成refernce_wrapper,std::thread以值形式接收此refernce_wrapper，然后通过refernce_wrapper的operator&()来达到隐式传递引用的效果
    t2.join();
    std::cout << "After t2 thread, x = " << x << std::endl;  // 输出 11

    //用法3:在std::bind中，通过std::ref来使得"参数"达到传递引用的效果
    int y=0;
    auto task1 = std::bind(Increment,y);
    task1();
    std::cout << "After task, y = " << y << std::endl;  // 输出 0

    auto task2 = std::bind(Increment, std::ref(y));
    task2();
    std::cout << "After task, y = " << y << std::endl;  // 输出 1

    TestSharedResource();

    //Todo:错误例子，std::ref要求接收一个左值，不能右值
    // int z=10;
    // auto refTmp = std::ref(10);//error

}