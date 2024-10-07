#include "list_initialzation.h"

#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <variant>
#include <vector>
#include <list>
#include <string>
#include <optional>
#include <functional>
#include <array>
#include <initializer_list>

class MyClass {
public:
    //会优先匹配初始化列表构造函数，不行就尝试匹配别的构造函数
    MyClass(std::initializer_list<int> list) {
        std::cout << "Initializer list constructor called." << std::endl;
        for (auto& item : list) {
            //item++;/只读
            data.push_back(item);
        }
    }

    MyClass(std::vector<int> vec)
        :data(vec)
    {
        std::cout << "vector constructor called." << std::endl;
    }

    MyClass(int x,int y)
        :data({x,y})
    {
        std::cout << "x="<<x<<" y="<<y<<std::endl;
    }

    void print() const {
        for (auto item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> data;
};

constexpr int sum(std::initializer_list<int> list) {
    int sum = 0;
    for (auto item : list) {
        sum += item;
    }
    return sum;
}

constexpr std::initializer_list<int> get_list()
{
    return {1,2,3,4,5};
}

//不能加constexpr，因为std::vector无法由编译器在编译时创建,是运行时创建的
//constexpr std::vector<int> get_vector()
std::vector<int> get_vector()
{
    return {1,2,3,4,5};
}

struct Person
{
    std::string name;
    int age;
};

void TestListInitAndInitialzerList()
{
    //std::initializer_list是一个轻量化容器
    //作为函数(包含构造函数和普通函数)传参
    MyClass obj{1,2};//列表初始化
    MyClass obj2={1,2};//列表初始化
    MyClass obj3(1,2);//常规构造函数
    MyClass obj4(std::vector<int>(1,2));//常规构造函数
    obj.print();
    
    //用于常量表达式
    constexpr auto sumRes = sum({1,2,3,4,5});
    std::cout << "sum="<<sumRes<<std::endl;

    constexpr auto list = get_list();//error
    // for(auto i:list)
    // {
    //     std::cout << i << " ";
    // }
    auto list2 = get_list();
    for(auto i:list2)
    {
        std::cout << i << " ";
    }
    auto vec = get_vector();

    //标准容器以及算法通常会使用 std::initializer_list 作为参数，以便在编译时进行优化。
    std::vector<int> vec2 = {1, 2, 3, 4, 5};

    //对于内置类型或者没有std::initializer_list作为构造函数参数的类，列表初始化会直接初始化对象
    int a{10};
    int b{};//0 初始化
    std::cout << "b="<<b<<std::endl;

    Person p{"cc",18};//因为类没有接受 std::initializer_list 作为构造函数参数，这里直接初始化成员
}
