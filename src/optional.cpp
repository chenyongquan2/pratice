#include "optional"

#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>
#include <optional>

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
        return m_data.at(key);
    }
    std::unordered_map<std::string, std::string> m_data;
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
}