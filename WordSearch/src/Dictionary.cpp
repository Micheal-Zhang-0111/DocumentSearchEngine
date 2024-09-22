#include"../include/Dictionary.h"

//删除中文字符中的标点或其他符号
string clearCNSymbol(const string &sentence){
    string ans;//返回没有标点符号的字符串
    int i = 0;
    while(sentence[i] != 0){
        //位运算判断是否为中文字符，中文字符在utf-8编码中占三个或四个字节(此处为3)
        if(sentence[i] & 0x80 && sentence[i] & 0x40 && sentence[i] & 0x20){
            int byteCount = 0;
            //判断中文字符占几个字节
            if(sentence[i] & 0x10){
                byteCount = 4;
            }
            else{
                byteCount = 3;
            }
            //将中文字符存入ans中
            for(int j = 0; j < byteCount; ++j){
                ans += sentence[i];
                ++i;
            }
        }
        //跳过非中文字符
        else if(sentence[i] & 0x80 && sentence[i] & 0x40){
            i += 2;
        }
        else{
            ++i;
        }
    }
    //移除标点符号
    auto it = std::remove_if(ans.begin(), ans.end(), [](char c){
        return ispunct(c);
    });
    ans.erase(it,ans.end());
    return ans;
}


Dictionary::Dictionary()
:_CNdirPath("../text_corpus/ChineseTextCorpus/"),
_ENdirPath("../text_corpus/EnglishTextCorpus/"),
_CNstopWordPath("../include/stop_words/stop_words_zh.txt"),
_ENstopWordPath("../include/stop_words/stop_words_eng.txt"),
_wordFrequencyDictPath("../WordFreq_Idx_Dir/wordFreqDir/wordFreq.dat"),
_relatedWordDictPath("../WordFreq_Idx_Dir/relatedWordDir/relatedWord.dat"),
_isEn(false)
{

}

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


//获取文件路径
void Dictionary::getFileList(){
    DIR *dir;//目录结构体
    string dirPath;//目录路径
    if(_isEn){
        dir = opendir(_ENdirPath.c_str());
        dirPath = _ENdirPath;
    }
    else{
        dir = opendir(_CNdirPath.c_str());
        dirPath = _CNdirPath;
    }
    struct dirent *pdirent;
    // pdirent = readdir(dir);
    while((pdirent = readdir(dir)) != NULL){
        if((string)pdirent->d_name != "." && (string)pdirent->d_name != ".."){
            _filePathList.push_back(dirPath + (string)pdirent->d_name);
        }
    }
    //***************可以注释******************
    for(auto &it : _filePathList){
        std::cout << it << std::endl;
    }
}

//创建词典
void Dictionary::buildDict(){
    cout<<"buildDict()"<<endl;
    getFileList();
    for(auto filepath : _filePathList){
        ifstream ifs;
        ifs.open(filepath,ios::in);//只读方式
        if(!ifs.good()){
            cout << "ifstream open error" << endl;
            return;
        }
        ifs.seekg(0,ios::beg);//定位到文件开头
        string line;
        while(getline(ifs,line)){
            ifstream stopWordifs;
            if(_isEn == false){//中文
                line = clearCNSymbol(line);//删除中文字符中的标点
                stopWordifs.open(_CNstopWordPath,ios::in);
            }
            else{//英文
                stopWordifs.open(_ENstopWordPath,ios::in);
            }
            if(!stopWordifs.good()){
                system("pause");
                exit(-1);
            }
            while(!stopWordifs.eof()){
                string stopWord;
                stopWordifs >> stopWord;
                _stopWordList.insert(stopWord);
            }
            vector<string> words = _splitTool->cut(line);
            for(string word : words){
                if(_stopWordList.find(word) != _stopWordList.end()){
                    continue;
                }
                for(vector<pair<string,int>>::iterator it = _WordFreqDict.begin();;it++){
                    if(it == _WordFreqDict.end()){//新单词
                        _WordFreqDict.push_back(make_pair(word,1));//插入新单词,次数为1
                        break;
                    }
                    else if((*it).first == word){
                        (*it).second++;//单词已存在，词频加1
                        break;
                    }
                }
                if(!_isEn){//中文
                    int letterIndex =  0;
                    while(letterIndex < word.size()){
                        int len = 0;
                        for(int j = 0; j < 6 && word[letterIndex] & (0x80 >> j); ++j){
                            len = j + 1;
                        }
                        _RelatedWordDict[word.substr(letterIndex,len)].insert(word);
                        letterIndex += len;
                    }
                }
                else{//英文
                    int letterIndex = 0;
                    while(letterIndex < word.size()){
                        int n = 1;//英文单词每个字符占用一个字节
                        _RelatedWordDict[word.substr(letterIndex,n)].insert(word);
                        letterIndex += n;//移动到下一个字符
                    }
                }
            }
        }
        ifs.close();
    }
}

//创建中文词典
void Dictionary::buildCnDict(){
    _isEn = false;
    _splitTool = new CppJieba();
    cout<<"buildCnDict()"<<endl;
    buildDict();
    storeWordFreqDict();
    storeRelatedWordDict();
}

//创建英文词典
void Dictionary::buildEnDict(){
    _isEn = true;
    _splitTool = new EnJieba();
    cout<<"buildEnDict()"<<endl;;
    buildDict();
    storeWordFreqDict();
    storeRelatedWordDict();
}

//将词典写入文件
void Dictionary::storeWordFreqDict(){
    cout << "storeWordFreqDict()" << endl;
    ofstream ofs;
    ofs.open(_wordFrequencyDictPath,ios::app|ios::out);
    cout<<"_wordFrequencyDictPath:"<<_wordFrequencyDictPath<<endl;
    if(!ofs.good()){
        cout << "ofstream open error" << endl;
        return;
    }
    for(auto it : _WordFreqDict){
        ofs << it.first << "\t" << it.second <<endl;
    }
    ofs.close();
}

//将关联词字典写入文件
void Dictionary::storeRelatedWordDict(){
    cout<<"RelatedWordDict()"<<endl;
    ofstream ofs;
    ofs.open(_relatedWordDictPath,ios::app|ios::out);
    cout<<"_relatedWordDictPath:"<<_relatedWordDictPath<<endl;
    if(!ofs.good()){
        cout << "ofstream open error" << endl;
        return;
    }
    for(auto it : _RelatedWordDict){
        ofs << it.first <<" ";
        for(auto it2 : it.second){
            ofs << it2 << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

/******************************************************/

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

//关联词查询
map<string,string> Dictionary::RelatedQuery(const string &word){
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
                set<string> inter = _redis.sinter(wordList[wordListIndex - 1],wordList[wordListIndex]);
                for(auto &it : inter){
                    result[it.c_str()] = _wordFreqDict[it.c_str()];
                }
                wordCurIndex += len;
            }
        }
    }
    else{//单个中文或英文字
        printf("word length = 1\n");
        vector<string> inter = _redis.smembers(word);//获取出现word的单词集合
        for(auto &it : inter){
            //插入在词频字典中的<字母,单词...>
            result[it.c_str()] = _wordFreqDict[it.c_str()];
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