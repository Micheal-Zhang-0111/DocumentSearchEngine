#pragma once

#include<string>
#include<vector>
#include<map>
#include<utility>
#include<memory>
#include<unordered_map>
#include"./MyRedis.h"

using std::vector;
using std::map;
using std::string;
using std::pair;
using std::shared_ptr;
using std::unordered_map;

struct RssPage
{
    string _title;
    string _link;
    string _content;
    string _keyword;
};

class WebPage
{
public:
    WebPage();
    WebPage(string xmlDirPath);

    void Redis2Memory();//将redis中的网页库和倒排索引表存储到内存中

    void getxmlPath();
    void buildPageLib();//建立网页库
    void buildPageOffsetLib();//建立网页偏移库

    void DelDuplicatePages();//删除重复网页,将重复文章flag标记为1,建立倒排索引表
    // void buildInvertIndexTable();//建立倒排索引表
  
    void storeLib2File();//将网页库、网页偏移库、倒排索引存储到文件中
    void storeLib2Redis();//将网页库和倒排索引存储到redis中

    
    void calDocidWordWeight(const vector<string> &keywordList,vector<string> &docidList, vector<vector<double>> &docidWeightList);
    void xml2Page(RssPage &page, int index);//将xml文件转换为struct RssPage格式,index为索引,不是docid
    void wordSimPage(string word,vector<RssPage> &pageList);//根据关键词查找相似网页


private:
    string _xmlDirPath; //存放xml文件的路径
    vector<string> _xmlPathList;//存放xml文件的路径的容器
    //网页库<<docid,文章内容>,0>//flag为1表示重复元素
    shared_ptr<map<int,pair<string,bool>>> _pageLib;
    // shared_ptr<vector<pair<string,int>>> _pageLib;
    //网页偏移库<docid，文件起始位置，文件的长度>
    map<int,pair<int,int>> _pageOffsetLib;
    //关键词及其归一化权重添加到倒排索引表InvertIndexTable中<查询词，集合<文档id,词语权重>>
    unordered_map<string,vector<pair<int,double>>> InvertIndexTable;

    MyRedis _redis;
};