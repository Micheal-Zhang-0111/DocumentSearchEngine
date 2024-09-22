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

    bool connect(string host,int port); //用于连接到Redis数据库
    vector<string> getAllKeys();//获取Redis数据库中所有的键
    string getValue(string key);//用Redis数据库中获取一个键对应的值
    void set(string key,string value);  //设置一个键值对到Redis数据库中
    void sadd(string key,string value); //将一个值添加到Redis数据库中的一个集合中
    vector<string> smembers(string key);//获取Redis数据库中一个集合的所有成员
    void select(string num);            //选择Redis数据库中的一个数据库
    std::set<string> sinter(string key1,string key2);//获取两个集合的交集

private:
    redisContext *_pConnect;
    redisReply *_pReply;
};
