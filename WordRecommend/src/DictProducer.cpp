#include"../include/DictProducer.h"

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

DictProducer::DictProducer(SplitTool *tool)
:_splitTool(tool),
_CNdirPath("../text_corpus/ChineseTextCorpus/"),
_ENdirPath("../text_corpus/EnglishTextCorpus/"),
_CNstopWordPath("../include/stop_words/stop_words_zh.txt"),
_ENstopWordPath("../include/stop_words/stop_words_eng.txt"),
_wordFrequencyDictPath("../WordFreq_Idx_Dir/wordFreqDir/wordFreq.dat"),
_relatedWordDictPath("../WordFreq_Idx_Dir/relatedWordDir/relatedWord.dat"),
_isEn(false)
{
}

//获取文件路径
void DictProducer::getFileList(){
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
void DictProducer::buildDict(){
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
void DictProducer::buildCnDict(){
    _isEn = false;
    cout<<"buildCnDict()"<<endl;
    buildDict();
    storeWordFreqDict();
    storeRelatedWordDict();
}

//创建英文词典
void DictProducer::buildEnDict(){
    _isEn = true;
    cout<<"buildEnDict()"<<endl;;
    buildDict();
    storeWordFreqDict();
    storeRelatedWordDict();
}

//将词典写入文件
void DictProducer::storeWordFreqDict(){
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
void DictProducer::storeRelatedWordDict(){
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