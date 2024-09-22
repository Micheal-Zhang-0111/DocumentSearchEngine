#pragma once
#include<queue>
#include<memory>
#include<vector>
#include<string>
#include"../SearchEngineHpp/WordDistance.h"
#include"../SearchEngineHpp/WordDictionary.h"

using std::priority_queue;
using std::to_string;

typedef struct RelatedResult{
    string _word; //单词
    int _freq;    //词频
    int _distance;//编辑距离
}RelatedResult;

class cmp
{
public:
    bool operator()(const RelatedResult &lhs,const RelatedResult &rhs)
    {   //lhs为输入的查询词，rhs为词库里的词
        //最小编辑距离越小，优先级越高
        if(lhs._distance > rhs._distance){
            return true;
        }
        //最小编辑距离越大，优先级越低
        else if(lhs._distance < rhs._distance){
            return false;
        }
        //最小编辑距离相同， 词频越高，优先级越高
        return lhs._freq < rhs._freq;
    }
};


class WordRecommander
{
public:
    WordRecommander();
    string doQuery(const string &queryword);
    vector<string> Top5Query(const string &queryword);//返回频率前5的推荐词
private:
    int distance(const string &lhs,const string &rhs);
private:
    Dictionary *_dict;
    priority_queue<RelatedResult,vector<RelatedResult>,cmp> _priQueue;
};