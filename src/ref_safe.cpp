#include "ref_safe.h"
#include <iostream>
#include <string>

static std::string& GeneratePass()
{
    std::string pwd = "123";
    pwd.append("123456");

    return pwd;
} 

void TestRefSafe()
{
    try
    {
        //Todo:像这种严重的非法内存问题，是无法被异常给捕获的；
        //当函数返回时，局部变量 pwd 已经被销毁。此时pwd为一个悬空引用
        std::string& pwd = GeneratePass();

        //会crash在下面这里
        std::cout << "pwd:" << pwd << std::endl;
    }
    catch (const std::exception& e)
    {
        //把异常信息给打印出来
        std::cout << "exception occured:" << e.what() << std::endl;
    }
    
}


