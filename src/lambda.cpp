#include "lambda.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <tuple>
#include <string_view>
#include <utility>
#include <thread>

//Todo:这里得加上这个namespace，不然asan运行时会检查报错:
// SUMMARY: AddressSanitizer: new-delete-type-mismatch D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win_delete_scalar_size_thunk.cpp:41 in operator delete(void *, unsigned __int64)   
// ==106976==HINT: if you don't care about these errors you may set ASAN_OPTIONS=new_delete_type_mismatch=0
namespace lambdaTest
{

struct Connection
{
public:
    Connection(size_t id):m_id(id)
    {
        setDefaultonTaskDoneCallable();
    }
    ~Connection() = default;

    void setDefaultonTaskDoneCallable()
    {
        //显示捕获this指针
        auto onTaskDoneFunc = [this]()
        {
            std::cout << "conn:" << m_id << " onTaskDone" << std::endl;
        };
        setonTaskDoneCallback(onTaskDoneFunc);
    }

    void setonTaskDoneCallback(std::function<void()> func)
    {
        //m_func = std::move(func);
        m_func = func;
    }
    
    void onTaskDone()
    {
        if(m_func)
        {
            m_func();
        }
    }

    std::function<void()> m_func;
    size_t m_id;
};


}

//Todo:不知道为啥这里会抛出异常？？？
template<typename Callable>
auto AddTask(Callable&& callable, std::shared_ptr<lambdaTest::Connection> spConn)
{
    std::thread t([callable = std::move(callable), spConn]() {
        callable();
        spConn->onTaskDone();
    });
    t.join();
}

void sum(int a, int b) {
   std::cout << "sum:" << a + b << std::endl;
}

struct ForbidCopy
{
    ForbidCopy(const ForbidCopy&) = delete;
    ForbidCopy(ForbidCopy&&) = default;
};

//Todo:测试对于引用本身的捕获
//Todo:注意pRightRef虽然是个右值引用，但是实际pRightRef变量本身它是个左值
void CaptureRef(std::unique_ptr<int>& p, ForbidCopy& f, std::unique_ptr<int>&& pRightRef)
{
    // std::unique_ptr<int>& p1 = p;//允许，引用只是相当于一个别名，没有去拷贝
    // std::unique_ptr<int> p2 = p;//不允许，这里会进行拷贝

    auto func = [&p]() {
        std::cout << "p:" << *p << std::endl;
    };
    func();

    //auto func2 = [p]() {};
    //这里捕获的本质相当:
    // std::unique_ptr<int>& p = xxx;
    // std::unique_ptr<int> m_p = p;//这里会尝试对unique_ptr进行拷贝构造，那就报错了

    auto func3 = [p = std::move(p)]() {
        std::cout << "p:" << *p << std::endl;
    };
    func3();

    //以一个我们自个的类为例，这个类是禁止拷贝的，但是我们可以通过lambda表达式来捕获这个类，并且捕获这个类的时候，
    // 我们使用的是值捕获，而不是引用捕获，这样就可以避免拷贝的开销。
    //auto func4 = [f]() {};
    //这里捕获的本质相当:
    // ForbidCopy& f = xxx;
    // ForbidCopy m_f = f;//这里会尝试对进行ForbidCopy拷贝构造，那就报错了

    auto func5 = [&f](){};
    auto func6 = [f=std::move(f)](){};

    //Todo:理解误区，命名的右值引用是左值
    //auto func7 = [pRightRef]() {};
    auto func8 = [&pRightRef]() {};
    auto func9 = [pRightRef = std::move(pRightRef)]() {};

}

struct MemberSharedPtrClass
{
public:
    std::shared_ptr<int> m_spInt;
    MemberSharedPtrClass(int x):m_spInt(std::make_shared<int>(x))
    {
    }

    void addAndPrintValue()
    {
        *m_spInt += 1;
        std::cout << "m_spInt:" << *m_spInt << std::endl;
    }
};

////////////////////////////////////////////
template<typename Callable>
auto AddTaskForMemberSharedPtrClass(Callable&& callable, std::shared_ptr<int> spTimes)
{
    callable();
    std::cout << *spTimes <<std::endl;
}


void testCaptureMoveSharedPtr()
{
    //成员变量是shared_ptr
    MemberSharedPtrClass obj(10);
    //Todo:下面这里其实会有风险，因为依赖于"参数评估顺序"，假如a部分参数先被执行，则会导致obj已经是个不可再访问的对象了
    //这里应该是编译器的"参数评估顺序"是：先b再a部分；
    //那么b部分会有一个shared_ptr指向一个已经析构的对象，然后a部分再执行，伴随着对象obj1的移动构造，会把对象obj1里面的shared_ptr成员也给移动构造给了objTmp了
    //这样在执行到AddTaskForMemberSharedPtrClass调用时，还是有两个shared_ptr执行这同一个int的；
    //所以关键再于理解shared_ptr的移动构造函数，不会对引用计数发生变化
    AddTaskForMemberSharedPtrClass(
        [obj=std::move(obj)]()mutable{
            obj.addAndPrintValue();
        },//a部分参数
        obj.m_spInt //b部分参数
    );
}


void TestLambda() {
    std::shared_ptr<lambdaTest::Connection> spConn = std::make_shared<lambdaTest::Connection>(0);
    auto task1 = [](){
        std::cout << "hello world" << std::endl;
    };
    //防止lambda表达式拷贝而降低效率，直接移动，但要注意这里被移动构造了之后，后面不能再去使用task1
    AddTask(std::move(task1), spConn);

    auto task2 = [x=10,y=20](){
        sum(x,y);
    };
    AddTask(std::move(task2), spConn);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    spConn->onTaskDone();
    spConn->setonTaskDoneCallback([](){
        std::cout << "conn onTaskDone" << std::endl;
    });
    spConn->onTaskDone();

    testCaptureMoveSharedPtr();

}
