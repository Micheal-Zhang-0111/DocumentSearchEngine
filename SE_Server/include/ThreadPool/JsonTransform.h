#pragma once

#include<iostream>
#include<string>
#include"../nlohmann/json.hpp"

using std::cout;
using std::endl;
using std::string;
using json = nlohmann::json;

//定义一个结构体存储数据
struct Data
{
    int length;//数据长度
    string data;//数据内容
};

class jsonTransform
{
public:
    jsonTransform(Data &data){
        _data = data;
    }

    void to_json(json &j){
        j = json{
            {"length",_data.length},
            {"data",_data.data}
        };
    }

    void from_json(const json &j){
        j.at("length").get_to(_data.length);
        j.at("data").get_to(_data.data);
    }

    void printData(){
        cout<<"length: "<<_data.length<<endl;
        cout<<"data: "<<_data.data<<endl;
    }

    int getDataLength(){
        return _data.length;
    }

    string getData(){
        return _data.data;
    }

private:
    Data _data;
};