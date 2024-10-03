#include "array.h"

#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>
#include <optional>
#include <functional>
#include <array>
#include <algorithm>

void TestArray()
{
    //Todo:与std::vector不同的是，std:::array的大小是固定的，不能动态增长或缩小。且在栈内存上
    std::string str="12344555";
    std::vector<std::string> vec;
    vec.push_back(str);
    vec.push_back(std::move(str));


    std::array<int,5> arr = {1,6,3,4,5};
    std::cout << "arr size="<<arr.size()<<std::endl;
    //输出某个元素
    std::cout << "arr[2]="<<arr.at(2)<<std::endl;
    std::cout << "arr[2]="<<arr[2]<<std::endl;

    std::sort(arr.begin(),arr.end());

    for(const auto&v:arr)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // .at()：提供运行时边界检查。如果尝试访问超出数组范围的元素，它会抛出 std::out_of_range 异常。
    // []：不进行边界检查。如果访问越界，会导致未定义行为，可能造成程序崩溃或其他不可预测的结果。

    try
    {
        std::cout << arr.at(arr.size()+1) <<std::endl;
    }
    catch (const std::exception&e)
    {
        std::cout << "exception occured, e="<<e.what()<<std::endl;
    }
}