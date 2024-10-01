#include "weak_ptr.h"
#include <cstring>
#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <thread>
#include <vector>

//应用于防止shared_ptr的循环引用，导致内存泄漏
struct Node
{
    std::shared_ptr<Node> m_spNext;//会导致node1和node2循环引用，导致内存泄漏
    std::weak_ptr<Node> m_spWeakNext;

    Node()
    {
        m_count++;
    }

    ~Node()
    {
        std::cout << "~Node()" << std::endl;
        m_count--;
    }

    static size_t GetCount() 
    {
        return m_count;
    }

    static size_t m_count;
};

size_t Node::m_count = 0;

//作为缓存
struct Cache
{
    std::unordered_map<int, std::weak_ptr<Node>> m_cache;

    std::shared_ptr<Node> GetNode(int key)
    {
        auto it = m_cache.find(key);
        if(it != m_cache.end())
        {
            if(auto resource = it->second.lock())
            {
                //shared_ptr的构造函数允许接受一个weak_ptr
                return resource;
            }
            else
            {
                m_cache.erase(it);
            }
        }
        //不存在，则创建
        auto spNode = std::make_shared<Node>();
        m_cache[key] = spNode;
        return spNode;
    }
};

//应用于观察者模式
struct Observer
{
    std::vector<std::weak_ptr<Node>> m_observers;
    void AddObserver(std::shared_ptr<Node> node)
    {
        m_observers.push_back(node);
    }

    void Nodify()
    {
        for(auto it = m_observers.begin(); it != m_observers.end();)
        {
            if(!it->expired())
            {
                auto observer = it->lock();
                //do something
                std::cout << "observer:" << observer << std::endl;
                ++it;
            }
            else
            {
                std::cout << "observer expired" << std::endl;
                //erase返回被删除的位置的下一个
                it = m_observers.erase(it);
            }
        }
    }
};

void TestWeakPtr() 
{
    {
        auto node1 = std::make_shared<Node>();
        auto node2 = std::make_shared<Node>();
        node1->m_spNext = node2;
        //node2->m_spNext = node1;
        node2->m_spWeakNext = node1;
    }
    std::cout << "Node::GetCount()=" << Node::GetCount() <<  std::endl;


    Cache cache;
    auto node = cache.GetNode(1);
    {
        auto node2 = cache.GetNode(2);
        std::cout << "Before node2 expire, Node::GetCount()=" << Node::GetCount() <<  std::endl;
    }
    std::cout << "After node2 expire, Node::GetCount()=" << Node::GetCount() <<  std::endl;

    /////////////
    Observer observer;
    auto node3 = std::make_shared<Node>();
    observer.AddObserver(node3);
    {
        auto node4 = std::make_shared<Node>();
        observer.AddObserver(node4);
        std::cout << "Before node4 expire, Node::GetCount()=" << Node::GetCount() <<  std::endl;
        observer.Nodify();
    }
    
    std::cout << "After node4 expire, Node::GetCount()=" << Node::GetCount() <<  std::endl;
     observer.Nodify();


    std::cout << "TestWeakPtr end" << std::endl;
    
}
