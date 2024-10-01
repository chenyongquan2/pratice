
#include "template.h"
#include "optional.h"
#include "singleton_impl.h"
//假如我们在下面的jthread.h里面#include "singleton_impl.h"了

#include "ref_safe.h"
#include "asan_detech.h"
#include "custom_deleter.h"
#include "callable.h"
#include "forward.h"
#include "universal_ref.h"
#include "bind.h"
#include "function.h"
#include "var_args_pkg.h"
#include "marco_inline_constexpr.h"
#include "constexpr.h"
#include "cast.h"
#include "string_view.h"
#include "lambda.h"
#include "weak_ptr.h"
#include "smart_ptr_mul_thread_safe.h"
#include "thread.h"

int main(){
    //模板例子
    //RegisterCommand();

    //std::optional例子
    //TestOptional();

    //单例模式例子
    //TestSingletonImpl();

    //函数返回引用，引用去接收一个生命周期即将销毁的变量的，不安全的写法
    //TestRefSafe();

    //测试asan的检测工具
    //TestAsanDetech();

    //智能指针指定自定义的Deleter
    //TestCustomDeleter();

    //测试通用引用
    //TestUniversalRef();

    //测试完美转发的重要性
    //TestForward();

    //可调用的概念
    //TestCallable();

    //测试bind
    //TestBind();

    //测试std::function
    //TestFunction();

    //测试tuple+可变参数包
    //TestVarArgsPkg();

    //测试宏，inline，constexpr
    //TestMarcoInlineConstexpr();

    ///测试constexpr
    //TestConstexpr();

    //测试类型转换cast
    //TestCast();

    //测试std::string_view
    //TestStringView();

    //测试lambda
    //TestLambda();

    //测试weak_ptr
    //TestWeakPtr();

    //测试智能指针线程安全性
    //TestSmartPtrMulThreadSafe();

    //测试线程detach和jthread
    TestThread();

    std::cout << "main end" << std::endl;

    return 0;
}