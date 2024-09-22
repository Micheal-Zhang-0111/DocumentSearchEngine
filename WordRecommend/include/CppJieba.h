#pragma once
#include<ctime>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cctype>
#include"../include/cppjieba/Jieba.hpp"
#include"../include/SplitTool.h"

using cppjieba::Jieba;

class CppJieba
:public SplitTool
{
public:
    CppJieba();
    ~CppJieba();

    virtual vector<string> cut(string &sentence);
private:
    vector<string> _words;//存放分词结果
    Jieba _jieba;//分词工具
};

class EnJieba
:public SplitTool
{
public:
    EnJieba();
    ~EnJieba();

    //清楚符号，将大写改为小写
    void clearSymble(string &sentence);
    virtual vector<string> cut(string &sentence);
private:
    vector<string> _words;//存放分词结果
};