#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<fstream>
#include"../include/SplitTool.h"
#include"../include/CppJieba.h"

using namespace std;

class DictProducer
{
public:
    DictProducer(SplitTool *tool);
    void getFileList();//获取文件路径
    void buildDict();//创建词典
    void buildCnDict();//创建中文词典
    void buildEnDict();//创建英文词典
    void storeIndex();//将索引表写入文件
    void storeDict();//将词典写入文件

private:
    string _CNdirPath;  //中文语料库路径
    string _ENdirPath;  //英文语料库路径
    string _CNstopWordPath;     //中文停用词路径
    string _ENstopWordPath;     //英文停用词路径
    string _wordFrequencyDictPath;  //词频字典路径
    string _wordIdxTablePath;       //词索引表路径(词在词频字典中出现的行号)
    vector<string> _filePathList;   //文件目录路径
    SplitTool *_splitTool;//分词工具
    vector<pair<string,int>> _WordFreqDict;//词频字典<单词,词频>
    map<string,set<int>> _WordIdxTable;//词索引表<单词,词频字典的行号>
    set<string> _stopWordList;         //停用词表
    bool _isEn;//是否是英文
};