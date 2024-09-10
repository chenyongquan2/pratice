#include <string>
#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>


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
std::enable_if<std::is_invocable_v<FunctionType, ExecutorType*, const std::string&>,void>::type
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

//////////////////////////////////////////////
template<typename T>
void calc()
{
    //typename example:
    typename T::value_type x;
}

void TemplateTest() {
    //typename example:
    calc<std::vector<int>>();
    calc<std::list<double>>();
    calc<std::vector<std::string>>();
}
//////////////////////////////////////////////
//const T&& 是一个通用引用（universal reference），它可以绑定到左值或右值。当传递左值时，T 会被推导为左值引用类型，当传递右值时，T 会被推导为非引用类型。
template<typename T>
//decltype(a+b) Append(const T&& a,const T&& b)
//decltype(auto) Append(const T&& a,const T&& b) 
T Append(const T& a,const T& b)
{
    return a+b;
    //return std::forward<T>(a) + std::forward<T>(b);
}

void TestAppend()
{
    int a=1;
    int b=2;
    auto c= Append<int>(a, b);
    std::cout << c << std::endl;

    auto d = Append(std::string("const T &&a"), std::string("const T &&b"));
    std::cout  << d << std::endl;
}
//////////////////////////////////////////////
//使用std::enable_if等SFINAE技术的例子
template<typename T>
std::enable_if<std::is_integral_v<T>,bool>::type
is_even(T t){
    return t%2==0;
}

template<typename T>
std::enable_if<std::is_floating_point_v<T>,bool>::type
is_even(T t){
    return std::fmod(t,2.0)==0;
}

void Test_EnableIf()
{
    std::cout << is_even(12) << std::endl;
    std::cout << is_even(2.0) << std::endl;
    std::cout << is_even(2.2) << std::endl;
}


//////////////////////////////////////////////
void RegisterCommand()
{
    static Server server;
    REGISTER(login);
    REGISTER(logout);
    REGISTER(create_account);

    //std::mem_fn example
    auto login = std::mem_fn(&Server::login);
    auto logout = std::mem_fn(&Server::logout);
    auto create_account = std::mem_fn(&Server::create_account);
    login(server,"123");
    logout(server,"123","123");
    create_account(server,"123","123","123");
    //command_map["login"]("123");

    //std::is_invocable_v example
    auto b1= std::is_invocable_v<decltype(&Server::login), Server*, const std::string&>;
    std::cout << b1 << std::endl;

    auto b2= std::is_invocable_v<decltype(&Server::login), Server*, const std::string&,int>;
    std::cout << b2 << std::endl;

    TemplateTest();

    TestAppend();

    Test_EnableIf();
}
