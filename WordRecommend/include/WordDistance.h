#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include"../include/Dictionary.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;


class WordDistance
{
public:
    WordDistance(){
        _dict = Dictionary::getInstance();
    }

    int editDistance(const string &lhs,const string &rhs);//计算两个单词之间的编辑距离
private:
    Dictionary *_dict;
};