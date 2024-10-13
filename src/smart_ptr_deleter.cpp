#include "smart_ptr_deleter.h"
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
#include <memory>
#include <thread>
#include <iostream>
#include <mutex>
#include <cstring> 

struct FileDeleter
{
    void operator()(FILE* fp)
    {
        fclose(fp);
        std::cout << "Close File successfully" << std::endl;
    }
};

void DoubleDeleter(double *p)
{
    std::cout << "Deleting double value:" << p << std::endl;
    if(p)
        delete p;
}

void TestSmartPtrDeleter() 
{
    {
        FILE* fp = fopen("test.txt", "w");
        std::unique_ptr<FILE, FileDeleter> spFile(fp);
        if (spFile)
        {
            std::cout << "File opened successfully." << std::endl;
            // Perform file operations
        }
        else
        {
            std::cerr << "Failed to open file: " << strerror(errno) << std::endl;
        }
    }

    {
        //Todo:std::make_unique不支持指定自定义删除器
        //auto spDouble = std::make_unique<double,deltype(DoubleDeleter)>(1234.123, DoubleDeleter);
        auto spDouble = std::make_unique<double>(1234.123);
        std::cout << "Double value:" << *spDouble << std::endl;
    }

    auto deleterLambda = [](int *p)
    {
        std::cout << "Deleting int value:" << *p << std::endl;
        if(p)
            delete p;
    };

    auto deleterLambdaVersion2 = [](int *p)
    {
        std::cout << "[deleterLambdaVersion2]Deleting int value:" << *p << std::endl;
        if(p)
            delete p;
    };

    {
        std::unique_ptr<int,decltype(deleterLambda)> spInt(new int(10), deleterLambda);
        std::cout << "spInt:" << *spInt << std::endl;
    }
    {
        std::shared_ptr<int> spInt(new int(10), deleterLambda);
        std::cout << "Before reset,spInt:" << *spInt << std::endl;

        //reset到另外一个对象时，可以指定为新的删除器
        spInt.reset(new int(100), deleterLambdaVersion2);
        std::cout << "After reset,spInt:" << *spInt << std::endl;
    }

    //Todo://std::make_unique不支持指定自定义删除器
    //{
    //     auto spStr = std::make_shared<std::string>("Hello, world!",[](std::string* pStr){
    //         std::cout << "Deleting String:" << *pStr << std::endl;
    //         if(pStr)
    //             delete pStr;
    //     });
    //     std::cout << "spStr:" << *spStr << std::endl;
    // }

}

