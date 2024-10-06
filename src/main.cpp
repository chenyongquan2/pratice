
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
#include "ref_wrapper.h"
#include "array.h"
#include "constructor_inherit.h"
#include "constructor_destructor_throw_exception.h"
#include "variant.h"
#include "static.h"
#include "thread_local.h"
#include "atomic_memory_order.h"

int main(){
    std::cout << "main start" << std::endl;
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
    //TestThread();

    //测试ref_wrapper的使用
    //TestRefWrapper();

    //测试std::array
    //TestArray();

    //测试构造函数继承
    //TestConstructorInherit();

    //测试构造函数和析构函数抛出异常
    //TestConstructorDestructorThrowException();

    //测试std::variant
    //TestVariant();

    //测试static的声明周期 与 初始化时机与 线程安全性
    //TestStatic();

    //测试thread_local
    //TestThreadLocal();

    //测试atomic和memory_order内存序，以及理解所谓内存屏障
    TestAtomicMemoryOrder();

    std::cout << "main end" << std::endl;

    return 0;
}