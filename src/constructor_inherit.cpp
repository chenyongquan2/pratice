#include "constructor_inherit.h"

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

class Base {
public:
    Base() : value(0) {
        std::cout << "Base default constructor" << std::endl;
    }
    
    Base(int v) : value(v) {
        std::cout << "Base parameterized constructor" << std::endl;
    }
    
    Base(const std::string& s) : name(s) {
        std::cout << "Base string constructor" << std::endl;
    }

protected:
    int value;
    std::string name;
};

class Derived : public Base {
public:
    using Base::Base; // 继承 Base 的构造函数
    
    Derived(int v, const std::string& s) :Base(v),extra(s) {
        std::cout << "Derived constructor" << std::endl;
    }

private:
    std::string extra;
};

void TestConstructorInherit()
{
    Derived d1;                  // 调用 Base 的默认构造函数
    Derived d2(42);              // 调用 Base(int) 构造函数
    Derived d3("Hello");         // 调用 Base(const std::string&) 构造函数
    Derived d4(10, "Extra");     // 调用 Derived 自己的构造函数
}