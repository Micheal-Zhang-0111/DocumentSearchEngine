#pragma once
#include<stdio.h>
#include<string.h>
#include<hiredis/hiredis.h>
#include<string>
#include<vector>
#include<iostream>
#include<set>
#include<unistd.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;

class MyRedis
{
public:
    MyRedis(){

    }

    ~MyRedis(){
        _pConnect = nullptr;
        _pReply = nullptr;
    }

    bool connect(string ip,int port);//用于连接到Redis数据库
    vector<string> getAllKeys();//获取Redis数据库中所有的键
    string getValue(string key);//用Redis数据库中获取一个键对应的值
    // string getCN(const string &key);//用Redis数据库中获取一个键对应的值
    // string getEn(string key);           //从Redis数据库中获取一个键对应的值
    void set(string key,string value);   //向Redis数据库中插入一个键值对
    void sadd(string key,string value);  //将一个值添加到Redis数据库中的一个集合中
    void select(string num);             //选择Redis中的一个数据库
    vector<string> smembers(string key); //获取Redis数据库中一个集合的所有成员
    std::set<string> sinter(string key1,string key2);//获取两个集合的交集

private:
    redisContext *_pConnect;
    redisReply *_pReply;
};
