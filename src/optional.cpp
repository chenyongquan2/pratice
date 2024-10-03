#include "optional.h"

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

struct Config
{
    Config()
    {
        m_data.insert(std::make_pair("name","cc"));
        m_data.insert(std::make_pair("age","18"));
    }
    std::optional<std::string> try_get_vallue(const std::string& key)
    {
        if(!m_data.count(key))
        {
            //无值状态
            return std::nullopt;
        }
        //std::optional 允许从可转换的类型进行隐式转换。
        return m_data.at(key);
    }
    std::unordered_map<std::string, std::string> m_data;
};

struct CanCopyAndMoveConstruct
{
    CanCopyAndMoveConstruct() = default;
    CanCopyAndMoveConstruct(const CanCopyAndMoveConstruct&) = delete;
    CanCopyAndMoveConstruct& operator=(const CanCopyAndMoveConstruct&) = delete;
    CanCopyAndMoveConstruct(CanCopyAndMoveConstruct&&) = delete;
    CanCopyAndMoveConstruct& operator=(CanCopyAndMoveConstruct&&) = delete;
};

// 对于更复杂的类型
class ComplexType {
public:
    ComplexType(int a, double b, std::string c) { /* ... */ }
};


void TestOptional()
{
    Config config;
    auto name = config.try_get_vallue("name");

    //has_value方法
    if(name.has_value())
    {
        //获取值的方法
        std::cout << name.value() << std::endl;
    }

    auto age = config.try_get_vallue("age");
    if(age)//更简洁的写法
    {
        //std::cout << age << std::endl;//error
        std::cout << *age << std::endl;//ok,更简洁的写法
    }

    auto age1 = config.try_get_vallue("age1");
    if(age1.has_value())
    {
        std::cout << age1.value() << std::endl;
    }
    else
    {
        std::cout << "age1 is not exist" << std::endl;
    }
    
    //默认值value_or方法
    auto score = config.try_get_vallue("score").value_or("0");
    std::cout << score << std::endl;

    //验证不可以绑定引用以及使用std::reference_wrapper来解决
    //Todo:下面会编译报错:std::optional 需要其模板参数是可以默认构造的，而引用不能被默认构造
    //std::optional<int&> opt;//编译报错
    //Todo:std::optional 需要其内容是可复制或可移动的。std::reference_wrapper 满足这些要求，而普通引用不满足
    int z = 10;
    std::optional<std::reference_wrapper<int>> optZ = std::ref(z);

    //测试对象不可移动也不可拷贝时
    //Todo std::optional要求所绑定的对象是可复制构造的或者可移动构造的
    std::string str="123";
    std::optional<std::string> optStr = str;//std::string是可复制构造的
    std::optional<std::string> optStr1 = std::move(str);//std::string是可移动构造的
    
    std::optional<CanCopyAndMoveConstruct> optCanCopyAndMoveConstruct;//编译通过，因为这里相当于 std::optional<CanCopyAndMoveConstruct> optCanCopyAndMoveConstruct=std::nullopt;
    CanCopyAndMoveConstruct obj;
    //optCanCopyAndMoveConstruct = obj;//编译报错:rc\optional.cpp(91): error C2679: 二元“=”: 没有找到接受“CanCopyAndMoveConstruct”类型的右操作数的运算符(或没有可接受的转换)

    //测试保存数组
    //std::optional<int[10]> optVec;//error
    std::optional<std::array<int,3>> optVec;//ok

    //测试结合std::in_place
    //使用std::in_place来 优化对象(可选类型（如std::optional）或变体类型（如std::variant）等类型)的构造过程
    //Todo://编译器会隐式地将 "1234" 转换为 std::string，然后用这个临时的 std::string 对象初始化 std::optional。这个过程中可能涉及一次额外的构造和移动操作
    std::optional<std::string> optStr2 = {"1234"};
    std::optional<std::string> optStr3{std::in_place,"1234"};//更好
    std::optional<ComplexType> optComplexType{std::in_place, 1,2,"1234"};

    // 下面这行无法直接编译
    // std::optional<ComplexType> optComplex2 = {1, 2.0, "three"};//无法将1, 2.0, "three"来隐式转换得到一个ComplexType对象
}