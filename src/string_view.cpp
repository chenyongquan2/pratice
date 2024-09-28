#include "string_view.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <string>
#include <tuple>
#include <string_view>
#include <utility>


void printLength(std::string_view sv)
{
    std::cout << "length:" << sv.length() << std::endl;
}

void printStdString(std::string& str)
{
    std::cout << "str:" << str << std::endl;
}

std::string_view GetSubString(std::string_view sv)
{
    return sv.substr(1,3);
}

void TestStringView()
{
    std::string str="hello world1";
    const char* cstr = "hello world2";
    char arr[] = "Boujour";

    //会隐式转换为std::string_view
    printLength(str);
    printLength(cstr);
    printLength(arr);

    //显示构造std::string_view
    std::string_view sv1=str;
    std::string_view sv2=cstr;
    std::string_view sv3=arr;

    //子字符串方法
    std::string_view sv4=sv1.substr(1,3);
    std::cout << "sv4:" << sv4 << std::endl;

    std::string_view sv5=sv1.substr(4);
    std::cout << "sv5:" << sv5 << std::endl;

    //remove_prefix只会去修改string_view的指向，不会去改变底层的字符串
    std::string_view tmp1 = sv1;
    std::string_view tmp2 = sv1;
    tmp1.remove_prefix(3);
    tmp2.remove_suffix(3);

    std::cout << tmp1 << ",size:" << tmp1.size() <<std::endl;
    std::cout << tmp2 << ",size:" << tmp2.size() <<std::endl;

    //借助.data() 转换回去std::string
    std::string strString = sv1.data();
    printStdString(strString);

    //注意string_view 不去管理对象的生命周期，因此如果底层字符串被销毁，则string_view 将包含未定义的内容。
    //因此，在使用 string_view 时，必须确保底层字符串在 string_view 对象的生命周期内保持有效。
    
    // std::string_view expireSv = GetSubString("temp string");
    // std::cout << expireSv << std::endl;
}