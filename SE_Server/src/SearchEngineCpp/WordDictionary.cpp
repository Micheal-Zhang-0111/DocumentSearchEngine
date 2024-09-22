#include"../../include/SearchEngineHpp/WordDictionary.h"

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

//从磁盘读取词典到_dict中
void Dictionary::readDiskDict(const string &wordFrequencyDictPath,const string &relatedWordDictPath){
    /********************   word frequency dict  **********************/
    string word;//存储单词
    string freq;//存储词频
    ifstream ifs(wordFrequencyDictPath);
    if(!ifs.good()){
        printf("open file error\n");
        return;
    }
    while(ifs >> word >> freq){
        // cout<<word<<" "<<freq<<endl;
        _wordFreqDict[word] = freq;
    }
    /********************   related Word dict  **********************/
    string letter;//存储字母
    string wordList;//存储在出现过该字母的单词
    ifstream ifs2(relatedWordDictPath);
    if(!ifs2.good()){
        printf("open file error\n");
        return;
    }
    string line;
    while(getline(ifs2,line)){
        istringstream iss(line);
        if(iss){
            iss >> letter;//word是字符
            while(iss){
                iss >> wordList;//后面出现的都是单词
                // cout<<letter<<" "<<wordList<<endl;
                _relatedWordDict[letter].insert(wordList);
            }
        }
    }

}

//初始化词典到redis数据库
void Dictionary::storeFile2Redis(const string &wordFrequencyDictDirPath,const string &relatedWordDictPath){
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
                _wordFreqDict[word] = freq;
                _redis.set(word,freq);
            }
        }
    }
    printf("load word frequency dict success\n");

    /********************   related Word dict  **********************/
    auto fileIdxList = getFileList(relatedWordDictPath);
    word.clear();
    string wordList;//存储在出现过该字母的单词
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
                    iss >> wordList;//后面出现的都是单词
                    _redis.sadd(word.c_str(),wordList.c_str());
                }
            }
        }
    }
    printf("load related Word Dict success\n");
}

//从redis数据库中读取词典到_wordFreqDict中
void Dictionary::readRedis2Dict(){
    MyRedis redis;
    _redis = redis;
    _redis.connect("127.0.0.1",6379);
    printf("connect redis success\n");
    /********************   word frequency dict  **********************/
    string word;//存储单词
    string freq;//存储词频
    //选择11号数据库(存储词频<单词，出现次数>)
    _redis.select("11");
    printf("select 2 db success\n");
    //将redis中的键值对存储到_wordFreqDict中
    vector<string> keys = _redis.getAllKeys();
    printf("keys.size() = %ld\n",keys.size());
    for(int i = 0; i < keys.size(); ++i){
        if(i<10){
            printf("keys[%d]:%s\n",i,keys[i].c_str());
        }
        freq = _redis.getValue(keys[i]);
        _wordFreqDict[keys[i].c_str()] = freq;
    }
    /********************   word dict  **********************/
    //存储出现该字母的单词
    vector<string> kvmap = _redis.getAllKeys();
    //选择12号数据库(存储索引<字母，单词....>)
    _redis.select("12");
    for(auto &key : kvmap){
        vector<string> values = _redis.smembers(key);
        for(auto &value : values){
            _relatedWordDict[key].insert(value);
        }
    }
}

//关联词查询(先在redis中找，然后在disk中找)
map<string,string> Dictionary::RelatedQuery(const string &word){
    _redis.connect("127.0.0.1",6379);
    printf("do related word query\n");
    map<string,string> result;//存储查询结果<单词，出现次数>
    _redis.select("12");

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
                //inter:两个字共同出现的单词集合

                // _redis.select("12");
                // set<string> inter = _redis.sinter(wordList[wordListIndex - 1],wordList[wordListIndex]);
                // _redis.select("11");
                set<string> inter;
                std::set_intersection(_relatedWordDict[wordList[wordListIndex - 1]].begin(),
                                    _relatedWordDict[wordList[wordListIndex - 1]].end(),
                                    _relatedWordDict[wordList[wordListIndex]].begin(),
                                    _relatedWordDict[wordList[wordListIndex]].end(),
                                    std::inserter(inter,inter.begin()));
                for(auto &it : inter){
                    result[it.c_str()] = _wordFreqDict[it.c_str()];
                    // result[it.c_str()] = _redis.getValue(it.c_str());
                }
                wordCurIndex += len;
            }
        }
    }
    else{//单个中文或英文字
        printf("word length = 1\n");
        // _redis.select("12");
        // vector<string> inter = _redis.smembers(word);//获取出现word的单词集合
        // _redis.select("11");
        set<string> inter = _relatedWordDict[word];
        for(auto &it : inter){
            //插入在词频字典中的<字母,单词...>
            result[it.c_str()] = _wordFreqDict[it.c_str()];
            // result[it.c_str()] = _redis.getValue(it.c_str());
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