#include "tuple.h"
#include <tuple>
#include <vector>
#include <string>
#include <iostream>

static void DealTupleByGet(std::tuple<std::string,int,double> data)
{
    auto name  = std::get<0>(data);
    auto age = std::get<1>(data);
    auto score = std::get<2>(data);

    std::cout << "name:" << name << ",age:" << age << ",score:" << score << std::endl;
}

static void DealTupleByStructureBind(std::tuple<std::string,int,double> data)
{
    auto [name,age,score] = data;
    std::cout << "name:" << name << ",age:" << age << ",score:" << score << std::endl;
}

void TestTuple()
{
    std::vector<std::tuple<std::string,int,double>> dataVec;
    for(int i=0;i<3;i++)
    {
        dataVec.emplace_back(std::make_tuple("key",i,double(i)));
    }
    for(auto i=0;i<dataVec.size();i++)
    {
        DealTupleByGet(dataVec[i]);
    }
    for(auto i=0;i<dataVec.size();i++)
    {
        DealTupleByStructureBind(dataVec[i]);
    }
}