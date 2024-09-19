
#include "template.h"
#include "optional.h"
#include "singleton_impl.h"
//假如我们在下面的jthread.h里面#include "singleton_impl.h"了

#include "jthread.h"
#include "ref_safe.h"
#include "asan_detech.h"

int main(){
    //模板例子
    //RegisterCommand();

    //std::optional例子
    //TestOptional();

    //std::jthread例子
    //TestJThread();

    //单例模式例子
    //TestSingletonImpl();

    //函数返回引用，引用去接收一个生命周期即将销毁的变量的，不安全的写法
    //TestRefSafe();

    //测试asan的检测工具
    TestAsanDetech();

    
    return 0;
}