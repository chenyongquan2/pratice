#include "variant.h"

#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <map>
#include <list>
#include <optional>
#include <functional>
#include <array>
#include <algorithm>
#include <vector>
#include <sstream>

#include <variant>
#include <type_traits>

// 定义配置项的可能类型


// 简单的配置解析器
class ConfigParser {
private:
    using ConfigValue = std::variant<int,double,std::string,bool>;
    std::map<std::string, ConfigValue> config;

public:
    void setConfig(const std::string&key, const ConfigValue& value) {
        config[key] = value;
    }

    template<typename T>
    T getConfig(const std::string&key)
    {
        return std::get<T>(config[key]);
    }

    void printConfig() {
        for (const auto& [key, value] : config) {
            std::cout << key << " = ";
            std::visit([](const auto& v) { std::cout << v; }, value);
            std::cout << std::endl;
        }
    }
};

void TestVariant() {
    //一个基本使用API的例子
    std::variant<int,double,std::string> data;
    data=10;
    std::cout << "data:" << std::get<int>(data) << std::endl;

    // data = 3.14;
    // std::cout << "浮点数: " << std::get<int>(data) << std::endl;
    
    data = "Hello, Variant!";
    std::cout << "字符串: " << std::get<std::string>(data) << std::endl;

    ConfigParser parser;

    // 设置不同类型的配置项
    parser.setConfig("max_connections", 100);
    parser.setConfig("pi", 3.14159);
    parser.setConfig("server_name", std::string("MyServer"));
    parser.setConfig("debug_mode", true);

    // 打印所有配置
    parser.printConfig();

    // 获取特定配置项
    int maxConn = parser.getConfig<int>("max_connections");
    std::string serverName = parser.getConfig<std::string>("server_name");

    std::cout << "Max connections: " << maxConn << std::endl;
    std::cout << "Server name: " << serverName << std::endl;
}

