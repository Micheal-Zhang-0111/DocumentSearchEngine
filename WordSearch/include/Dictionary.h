#pragma once

#include<vector>
#include<map>
#include<set>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unordered_map>
#include<fstream>
#include<dirent.h>
#include<sstream>
#include<unistd.h>
#include<utility>
#include<iostream>
#include<string>
#include"../include/MyRedis.h"
#include"../include/SplitTool.h"
#include"../include/CppJieba.h"

using std::vector;
using std::map;
using std::unordered_map;
using std::ifstream;
using std::istringstream;
using std::pair;
using std::string;
using std::make_pair;
using std::set;
using std::ofstream;
using std::ios;

class Dictionary
{
public:
    void getFileList();//获取文件路径
    void buildDict();//创建词典
    
    void buildCnDict();//创建中文词典
    void buildEnDict();//创建英文词典

    void storeWordFreqDict();//将词频词典写入文件
    void storeRelatedWordDict();//将关联词字典写入文件

    static Dictionary *getInstance();
    static void destroy();

    void storeFile2Redis(const string &wordFrequencyDictPath,const string &relatedWordDictPath);//初始化词典到redis数据库
    void readRedis2Dict();//从redis数据库中读取词典到_dict中

    map<string,string> RelatedQuery(const string &word);//关联词查询 

    vector<string> getFileList(const string &dirPath);//存储词典文件路径的列表
    bool isChinese(const string &word);//判断字符是否为中文(utf-8编码)

private:
    Dictionary();

    ~Dictionary(){
        _redis.~MyRedis();
    }
private:
    MyRedis _redis;

    string _CNdirPath;  //中文语料库路径
    string _ENdirPath;  //英文语料库路径
    string _CNstopWordPath;     //中文停用词路径
    string _ENstopWordPath;     //英文停用词路径
    string _wordFrequencyDictPath;  //词频字典路径
    string _relatedWordDictPath;    //字母对应单词字典路径(词在词频字典中出现的单词)
    
    vector<string> _filePathList;   //文件目录路径
    SplitTool *_splitTool;//分词工具

    vector<pair<string,int>> _WordFreqDict;//词频字典<单词,词频>
    map<string,set<string>> _RelatedWordDict;//关联词字典<字母,相关单词....>
    set<string> _stopWordList;         //停用词表
    bool _isEn;//是否是英文
    //*********************后期优化，这里是全放到_dict中本地计算 */
    map<string,string> _wordFreqDict;//存储单词和词频  <单词,词频>
    map<string,set<string>> _relatedWordDict;//存储关联词   <字母,单词...>
    vector<string> _queryResult;//存储查询结果
    static Dictionary *_pInstance;//单例模式是一种设计模式，它确保一个类只有一个实例
};