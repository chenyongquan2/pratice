#include "function.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <stop_token>
#include <thread>


struct Connection:public std::enable_shared_from_this<Connection>
{
public:
    Connection(size_t uid,std::function<void(std::shared_ptr<Connection>)> on_close_callback)
        :m_on_close_callback(on_close_callback)
        ,m_uid(uid)
    {
        //std::cout << "Connection constructed" << std::endl;
    }
    ~Connection()
    {
        std::cout << "Connection destructed" << std::endl;
    }

    //模拟客户端主动断开后(客户端只关闭了socket的写端，读端还保留着)，服务端需要把任务处理完成，然后把响应都发给客户端，然后再关闭和客户端的连接；
    void LasyClose()
    {
        //moni关闭与客户端的连接socket
        //socket->close();

        //通知管理着connnection的上一层server层，例如从连接池里面移除掉，从而释放掉conn的内存
        std::cout << "uid:" << m_uid << " all of task is done, Try to notify the Server the disconnect with the client..." <<std::endl;
        if(m_on_close_callback)
            m_on_close_callback(shared_from_this());
    }

    void Process(int taskId)
    {
       //std::cout << "Process taskId:" << taskId << std::endl;

       //返回响应
       std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void EventLoopThread()
    {
        //模拟从客户端上读取请求
        std::this_thread::sleep_for(std::chrono::seconds(3));

        //处理请求
        Process(1);
        Process(2);
        Process(3);

        //Todo: 收到客户端的主动断开信号
        //模拟客户端主动断开后(客户端只关闭了socket的写端，读端还保留着)，服务端需要把任务处理完成，然后把响应都发给客户端，然后再关闭和客户端的连接；
        LasyClose();
    }

    std::function<void(std::shared_ptr<Connection>)> m_on_close_callback;

public:
    size_t m_uid;
};

struct TcpServer
{
public:
    void Server(int socketId)
    {
        std::shared_ptr<Connection> spConn = std::make_shared<Connection>(socketId,[this](std::shared_ptr<Connection> spConn){
            std::cout << "Server Is Trying to close the connection,uid:" << spConn->m_uid << std::endl;
            this->OnDisconnection(spConn);
        });
        AddConnection(spConn);

        //模拟线程池的调度任务
        auto connEventLoop = [spConn](){
            spConn->EventLoopThread();
        };
        std::jthread jtask(connEventLoop);
        jtask.detach();
    }

    bool IsAllConnectionClosed()
    {
        return m_conn_map.empty();
    }

    
private:
    void OnDisconnection(std::shared_ptr<Connection> spConn)
    {
        std::cout << "OnDisconnection uid:" << spConn->m_uid << std::endl;
        RemoveConnection(spConn);
    }
    void AddConnection(std::shared_ptr<Connection> spConn)
    {
        std::cout << "AddConnection uid:" << spConn->m_uid << " from its map" << std::endl;
        m_conn_map.insert({spConn->m_uid,spConn});
    }

    void RemoveConnection(std::shared_ptr<Connection> spConn)
    {
        std::cout << "RemoveConnection uid:" << spConn->m_uid  << " from its map"<< std::endl;
        m_conn_map.erase(spConn->m_uid);
    }

    std::unordered_map<size_t, std::shared_ptr<Connection>> m_conn_map;
    
};

void TestFunction()
{
    TcpServer server;
    //模拟客户端连接上来
    server.Server(1000);
    server.Server(1001);
    server.Server(1002);

    //模拟服务器loop
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "Server Exit, IsAllConnectionClosed:" << server.IsAllConnectionClosed() << std::endl;

}
