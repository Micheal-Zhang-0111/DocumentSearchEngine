#pragma once

#include<vector>
#include<map>
#include<unordered_map>
#include<fstream>
#include<dirent.h>
#include<sstream>
#include<unistd.h>
#include<utility>
#include<string>
#include"../include/MyRedis.h"

using std::vector;
using std::map;
using std::unordered_map;
using std::ifstream;
using std::istringstream;
using std::pair;
using std::string;
using std::make_pair;

class Dictionary
{
public:
    static Dictionary *getInstance();
    static void destroy();

    void initDict(const string &wordFrequencyDictPath,const string &wordIdxTablePath);//初始化词典到redis数据库
    void readRedis2Dict();//从redis数据库中读取词典到_dict中
    map<string,string> RelatedQuery(const string &word);//关联词查询 
    vector<string> getFileList(const string &dirPath);//存储词典文件路径的列表
    bool isChinese(const string &word);//判断字符是否为中文(utf-8编码)

private:
    Dictionary(){

    }
    ~Dictionary(){
        _redis.~MyRedis();
    }
private:
    MyRedis _redis;

    //*********************后期优化，这里是全放到_dict中本地计算 */
    vector<pair<string,string>> _dict;//存储单词和词频

    vector<string> _queryResult;//存储查询结果
    static Dictionary *_pInstance;//单例模式是一种设计模式，它确保一个类只有一个实例
};