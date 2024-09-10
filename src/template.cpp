#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>

class Server{
public:
    void login(const std::string& request_uid)
    {
        std::cout << "login one param" << std::endl;
        //one param
    }
    void logout(const std::string& request_uid,const std::string& session_uid)
    {
        //two param
        std::cout << "logout two param" << std::endl;
    }
    void create_account(const std::string& request_uid, const std::string& session_uid, const std::string& data)
    {
        //three param
        std::cout << "create_account three param" << std::endl;
    }
};

//is_invocable_v 检查某个可调用对象是否可以被调用，且可以接受指定的参数

template <typename FunctionType, typename ExecutorType, typename Function>
std::enable_if<std::is_invocable_v<FunctionType, ExecutorType&, const std::string&>,void>::type
register_func(const std::string& api_name,Function&& func, ExecutorType executor)
{
    std::cout << "register_func one param" << std::endl;
}

template <typename FunctionType, typename ExecutorType, typename Function>
std::enable_if<std::is_invocable_v<FunctionType, ExecutorType&, const std::string&, const std::string&>,void>::type
register_func(const std::string& api_name,Function&& func, ExecutorType executor)
{
    std::cout << "register_func two param" << std::endl;
}

template <typename FunctionType, typename ExecutorType, typename Function>
std::enable_if<std::is_invocable_v<FunctionType, ExecutorType&, const std::string&, const std::string&, const std::string&>,void>::type
register_func(const std::string& api_name,Function&& func, ExecutorType executor)
{
    std::cout << "register_func three param" << std::endl;
}

#define REGISTER(api_name) \
register_func<decltype(&Server::api_name),Server>(#api_name, std::mem_fn(&Server::api_name), server);


void RegisterCommand()
{
    // std::unordered_map<std::string, std::function<class Fty>> 
    static Server server;
    REGISTER(login);
    REGISTER(logout);
    REGISTER(create_account);
}