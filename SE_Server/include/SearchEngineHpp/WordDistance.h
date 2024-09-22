#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

/*
    单词距离计算
    返回两个单词间的编辑距离
*/

typedef struct CandidateResult
{
    string _word;//单词
    int _freq;   //词频
    int _dis;    //距离
}CandidateResult;

class WordDistance
{
public:
    WordDistance();

    bool isChinese(const string &word);//判断字符是否为中文(utf-8编码)
    int editDistance(const string &lhs,const string &rhs);
};
