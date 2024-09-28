#include "cast.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <tuple>
#include <string_view>
#include <utility>

class Animal {
public:
    virtual void makeSound() { std::cout << "Animal sound" << std::endl; }
};

class Dog : public Animal {
public:
    void makeSound() override { std::cout << "Woof" << std::endl; }
    void wagTail() { std::cout << "Dog wagging tail x:" << x << std::endl; }
    int x=10;
};

class Cat : public Animal {
public:
    void makeSound() override { std::cout << "Meow" << std::endl; }
    void purr() { std::cout << "Cat purring" << std::endl; }
    //void wagTail() { std::cout << "Car wagging tail" << std::endl; }
};


void TestCast()
{
    Animal* animal = new Cat();
    
    // 不安全的向下转换
    Dog* dog = static_cast<Dog*>(animal);
    //will cause undefined behavior
    //dog->wagTail(); // 潜在的未定义行为
    
    delete animal;


    //dynamic_cast的使用
    //指针类型的转换失败例子
    Animal* pAnimal = new Animal();
    Dog* pDog = dynamic_cast<Dog*>(pAnimal);
    if (pDog) {
        
        pDog->makeSound();
    } else {
        //return nullptr
        std::cout << "Conversion failed" << std::endl;
    }

    Animal animal2;
    try
    {
        Dog& dog2= dynamic_cast<Dog&>(animal2);
    }
    catch (const std::exception& e)
    {
        //Bad dynamic_cast intead of nullptr because of empty reference
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

   
}