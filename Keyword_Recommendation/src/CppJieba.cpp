#include"../include/CppJieba.h"

/*
    加载分词库所需要资源
    DICT_PATH：     这是分词词典的路径。分词词典包含了一系列词语和对应的词性，用于将文本切分成有意义的词语。
    HMM_PATH：      这是HMM（隐马尔可夫模型）模型的路径。HMM模型用于处理中文分词中的未登录词和歧义问题，帮助更准确地进行分词。
    USER_DICT_PATH：这是用户自定义词典的路径。用户可以根据自己的需求添加一些特定的词语，以便在分词过程中更好地识别这些词语。
    IDF_PATH：      这是IDF（逆文档频率）的路径。IDF用于计算一个词语在整个语料库中的重要性，帮助过滤掉一些常见的无意义词语。
    STOP_WORD_PATH：这是停用词的路径。停用词是一些常见的无意义词语，如介词、连词等，它们在分词过程中通常被过滤掉，以提高分词的准确性和效率。
*/
const char* const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";      // 分词词典路径
const char* const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";         // HMM模型路径
const char* const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";   // 用户自定义词典路径
const char* const IDF_PATH = "../include/cppjieba/dict/idf.utf8";               // IDF路径
const char* const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";  // 停用词路径

CppJieba::CppJieba()
:_jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
{

}

CppJieba::~CppJieba(){

}

vector<string> CppJieba::cut(string &sentence){
    _words.clear();//清空存储分词结果的容器
    _jieba.CutForSearch(sentence, _words);//使用jieba分词器对字符串进行分词
    return _words;//返回分词结果
}

EnJieba::EnJieba(){

}

EnJieba::~EnJieba(){

}

void EnJieba::clearSymble(string &sentence){
    for(unsigned int i = 0; i < sentence.size(); ++i){
        if(sentence[i] >= 'A' && sentence[i] <= 'Z'){
            sentence[i] = tolower(sentence[i]); //大写字母转换为小写字母 
        }
        else if(sentence[i] == '\n' || sentence[i] == ' ' || (sentence[i] >= 'a' && sentence[i] <= 'z')){
            continue;
        }
        else{//非字母字符替换为空格
            sentence[i] = ' ';
        }
    }
}

vector<string> EnJieba::cut(string &sentence){
    clearSymble(sentence);//去除符号并转小写字母
    _words.clear();//清空存储分词结果的容器
    std::istringstream iss(sentence.c_str());//将字符串转换为输入流
    string word;
    while(iss >> word){
        _words.push_back(word);//将分词结果存入容器
    }
    return _words;//返回分词结果
}