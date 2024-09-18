
#include "template.h"
#include "optional.h"
#include "singleton_impl.h"
//假如我们在下面的jthread.h里面#include "singleton_impl.h"了

#include "jthread.h"

int main(){
    //模板例子
    //RegisterCommand();

    //std::optional例子
    //TestOptional();

    //std::jthread例子
    //TestJThread();

    //单例模式例子
    TestSingletonImpl();

    
    return 0;
}