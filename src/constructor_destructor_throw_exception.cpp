#include "constructor_destructor_throw_exception.h"

#include <memory>
#include <stdexcept>
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
#include <exception>

struct Member
{
    Member()
    {
        std::cout << "Member()" << std::endl;
    }
    ~Member()
    {
        std::cout << "~Member()" << std::endl;
    }
};

struct ConstructorDestructorThrowException
{
    ConstructorDestructorThrowException()
    {
        std::cout << "ConstructorDestructorThrowException() Begin" << std::endl;
        m_pMember = new Member();
        m_spMember = std::make_shared<Member>();
        //模拟构造函数中如果抛出了异常的case
        //throw std::runtime_error("ConstructorDestructorThrowException() throw exception");
        
        std::cout << "ConstructorDestructorThrowException() End" << std::endl;
    }

    ~ConstructorDestructorThrowException()
    {
        try
        {
            std::cout << "~ConstructorDestructorThrowException() Begin" << std::endl;
            throw std::runtime_error("~ConstructorDestructorThrowException() throw exception");
            if(m_pMember)
                delete m_pMember;
        }
        catch (const std::exception& e) 
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }
        
        std::cout << "~ConstructorDestructorThrowException() End" << std::endl;
    }
    void CleanUp()
    {
        if(m_pMember)
            delete m_pMember;
    }
    Member *m_pMember = nullptr;
    std::shared_ptr<Member> m_spMember;
};

void TestConstructorDestructorThrowException()
{
    try 
    {
        ConstructorDestructorThrowException c;
        //c.CleanUp();
    } 
    catch (const std::exception& e) 
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    std::cout << "==================" << std::endl;
}