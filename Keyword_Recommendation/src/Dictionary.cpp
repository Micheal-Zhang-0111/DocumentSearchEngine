#include"../include/Dictionary.h"

//静态成员函数类外实现
Dictionary *Dictionary::_pInstance = nullptr;

Dictionary *Dictionary::getInstance(){
    if(_pInstance == nullptr){
        _pInstance = new Dictionary();
    }
    return _pInstance;
}

void Dictionary::destroy(){
    if(_pInstance != nullptr){
        delete _pInstance;
        _pInstance = nullptr;
    }
}

//初始化词典到redis数据库
void Dictionary::initDict(const string &wordFrequencyDictDirPath,const string &wordIdxTableDirPath){
    printf("init dict begin()\n");
    MyRedis redis;
    _redis = redis;
    _redis.connect("127.0.0.1",6379);
    printf("connect redis success\n");

    /********************   word frequency dict  **********************/
    //获取词频文件列表
    auto fileList = getFileList(wordFrequencyDictDirPath);
    string word;//存储单词
    string freq;//存储词频
    //选择11号数据库(存储词频<单词，出现次数>)
    _redis.select("11");
    printf("select 11 db success\n");
    for(auto file : fileList){
        printf("file:%s\n",file.c_str());
        std::ifstream ifs(file);
        if(!ifs.good()){
            printf("open file error\n");
            return;
        }
        string line;
        while(getline(ifs,line)){
            std::istringstream iss(line);
            if(iss >> word >> freq){
                _dict.push_back(make_pair(word,freq));
                // _redis.set(word,freq);
            }
        }
    }
    printf("load word frequency dict success\n");

    /********************   word index dict  **********************/
    auto fileIdxList = getFileList(wordIdxTableDirPath);
    word.clear();
    string index;//存储在词频表中出现的行号
    //选择12号数据库(存储索引<字母，单词>)
    _redis.select("12");
    for(auto file : fileIdxList){
        ifstream ifs(file);
        if(!ifs.good()){
            printf("open file error\n");
            return;
        }
        string line;
        while(getline(ifs,line)){
            istringstream iss(line);
            if(iss){
                iss >> word;//word是字符
                while(iss){
                    iss >> index;//后面出现的都是行号
                    _redis.sadd(word.c_str(),index.c_str());
                }
            }
        }
    }
    printf("load word index dict success\n");
}

//从redis数据库中读取词典到_dict中
void Dictionary::readRedis2Dict(){
    MyRedis redis;
    _redis = redis;
    _redis.connect("127.0.0.1",6379);
    printf("connect redis success\n");
    /********************   word frequency dict  **********************/
    string word;//存储单词
    string freq;//存储词频
    //选择2号数据库(存储词频<单词，出现次数>)
    _redis.select("2");
    printf("select 2 db success\n");
    //将redis中的键值对存储到_dict中
    vector<string> keys = _redis.getAllKeys();
    printf("keys.size() = %ld\n",keys.size());
    for(int i = 0; i < keys.size(); ++i){
        if(i<10){
            printf("keys[%d]:%s\n",i,keys[i].c_str());
        }
        freq = _redis.getValue(keys[i]);
        _dict.push_back(make_pair(keys[i].c_str(),freq));
    }
    /********************   word index dict  **********************/
    //按照redis中存放顺序，重新存储索引行号
    vector<string> kvmap = _redis.getAllKeys();
    // string index;//存储在词频表中出现的行号
    //选择3号数据库(存储索引<单词，行号>)
    _redis.select("3");
    for(int i = 0; i < keys.size(); ++i){
        vector<string> lineNumber = _redis.smembers(keys[i]);
        for(auto &it : lineNumber){
            _redis.sadd(keys[i],it);
        }
    }
}

//关联词查询
map<string,string> Dictionary::RelatedQuery(const string &word){
    printf("do related word query\n");
    map<string,string> result;//存储查询结果<单词，出现次数>

    vector<string> wordList;//存储word中的每一个字符
    int wordCurIndex = 0;//记录当前字符的位置
    int wordListIndex = -1;//记录当前使用wordlist中的索引
    int len = 1;//标记中英文单个字符长度
    if(isChinese(word)){//中文
        len = 3;
        printf("chinese\n");
    }
    else{//英文
        len = 1;
        printf("english\n");
    }
    if( (len == 3 && word.length() > 3) || (len == 1 && word.length() > 1)){//中文或英文字数大于1
        printf("word length > 1\n");
        while(wordCurIndex < word.length()){
            string letter = word.substr(wordCurIndex,len);
            wordList.push_back(letter);
            ++wordListIndex;
            if(wordCurIndex == 0){
                //第一次不做交集运算
                wordCurIndex += len;
                continue;
            }
            else{
                //inter:两个字共同出现的行号集合
                set<string> inter = _redis.sinter(wordList[wordListIndex - 1],wordList[wordListIndex]);
                for(auto &it : inter){
                    result.insert(make_pair(_dict[atoi(it.c_str())-1].first, _dict[atoi(it.c_str())-1].second));
                }
                wordCurIndex += len;
            }
        }
    }
    else{//单个中文或英文字
        printf("word length = 1\n");
        vector<string> inter = _redis.smembers(word);//获取word的行号集合
        for(auto &it : inter){
            //插入在词频字典中的<词,出现次数>,索引为行号
            result.insert(make_pair(_dict[atoi(it.c_str())-1].first, _dict[atoi(it.c_str())-1].second));
        }
    }
    return result;
}

//存储词典文件路径的列表
vector<string> Dictionary::getFileList(const string &dirPath){
    cout<<"dirPath:"<<dirPath<<endl;
    DIR *dir = opendir(dirPath.c_str());
    struct dirent *pdirent;
    vector<string> fileList;
    while((pdirent = readdir(dir)) != nullptr){
        if(pdirent->d_type == DT_REG){//当前文件是普通文件
            fileList.push_back(dirPath + (string)pdirent->d_name);
        }
    }
    for(auto &it : fileList){
        cout<<it<<endl;
    }
    return fileList;
}

//判断字符是否为中文(utf-8编码)
bool Dictionary::isChinese(const string &word){
    if(word[0] != 0 && word[0] & 0x80 && word[0] & 0x40 && word[0] & 0x20){
        return true;
    }
    else{
        return false;
    }
}