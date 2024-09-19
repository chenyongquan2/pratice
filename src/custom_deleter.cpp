#include "custom_deleter.h"
#include <cstring>
#include <iostream>
#include <memory>

template<typename ManagerAPI,typename MtStruct>
//1.std::shared_ptr<MtStruct>虽然没有指定Deleter的类型，但是会自动类型推导为此lambda的类型
//2.std::shared_ptr内部含有一个控制块，控制块内部含有Deleter作为成员。
//std::shared_ptr<MtStruct> make_shared(ManagerAPI *pManagerAPI, MtStruct *pMtStruct)
auto make_shared(ManagerAPI *pManagerAPI, MtStruct *pMtStruct)
{
    return std::shared_ptr<MtStruct>(pMtStruct,[pManagerAPI](MtStruct *pMtStruct){
        //不止是调用delete
        pManagerAPI->MemFree(pMtStruct);
    });
}

struct UserStruct
{
    UserStruct(std::string name):m_name(name)
    {
        std::cout << "UserStruct constructed: " << m_name << std::endl;
    }
    ~UserStruct()
    {
        std::cout << "UserStruct destructed: " << m_name << std::endl;
    }
    std::string m_name;
};

struct GroupStruct
{
    GroupStruct(std::string name):m_name(name)
    {
        std::cout << "GroupStruct constructed: " << m_name << std::endl;
    }
    ~GroupStruct()
    {
        std::cout << "GroupStruct destructed: " << m_name << std::endl;
    }
    std::string m_name;
};

struct ManagerAPI
{
public:
    std::shared_ptr<UserStruct> CreateUser()
    {
        auto *pUser =  new UserStruct("cc");
        return make_shared(this, pUser);
    }

    std::shared_ptr<GroupStruct> CreateGroup()
    {
        auto *pGroup =  new GroupStruct("group1");
        return make_shared(this, pGroup);
    }

    template<typename MtStruct>
    void MemFree(MtStruct *pMtStruct)
    {
        //不止是调用delete，例如还去做一些别的工作，例如这里打日志
        std::cout << "ManagerAPI::MemFree() pMtStruct=" << pMtStruct << std::endl;
        delete pMtStruct;
    }

};

void TestCustomDeleter() {
    ManagerAPI managerAPI;
    //auto pUser = managerAPI.CreateUser();
    {
        //1.std::shared_ptr<MtStruct>虽然没有指定Deleter的类型，但是会自动类型推导为此lambda的类型
        std::shared_ptr<UserStruct> pUser2 = managerAPI.CreateUser();
    }
    auto pGroup = managerAPI.CreateGroup();
}
