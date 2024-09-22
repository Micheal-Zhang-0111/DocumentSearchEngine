#include"../../include/SearchEngineHpp/SE_WordRecommand.h"

WordRecommander::WordRecommander(){
    _dict = Dictionary::getInstance();
    // _dict->storeFile2Redis("../WordFreq_Idx_Dir/wordFreqDir/","../WordFreq_Idx_Dir/relatedWordDir/");
    // _dict->readRedis2Dict();
    _dict->readDiskDict("../dictDatabase/wordDict/wordFreq.dat","../dictDatabase/wordDict/relatedWord.dat");
}

string WordRecommander::doQuery(const string &queryword){
    RelatedResult _relatedResult;
    while(!_priQueue.empty()){
        _priQueue.pop();
    }
    printf("queryword:%s\n",queryword.c_str());
    map<string,string> _queryResult = _dict->RelatedQuery(queryword);//查询结果<单词,出现次数>
    int cnt = 0;//候选词个数
    for(auto it : _queryResult){
        _relatedResult._word = it.first;
        _relatedResult._freq = atoi(it.second.c_str());
        _relatedResult._distance = distance(queryword,it.first);
        _priQueue.push(_relatedResult);
    }
    string kMsg;
    kMsg+="\n关键词推荐\n";
    kMsg+="关键词\t\t\t\t词频\t\t\t\t最小编辑距离\n";
    while(!_priQueue.empty() && cnt < 10){
        //printf("%s \t\t %d \t\t %d\n",_priQueue.top()._word.c_str(),_priQueue.top()._freq,_priQueue.top()._distance);
        kMsg+=_priQueue.top()._word+"\t\t\t\t"+to_string(_priQueue.top()._freq)+"\t\t\t\t"+to_string(_priQueue.top()._distance)+"\n";
        _priQueue.pop();
        ++cnt;
    }
    return kMsg;
}

vector<string> WordRecommander::Top5Query(const string &queryword){
    vector<string> top5words;
    RelatedResult _relatedResult;
    while(!_priQueue.empty()){
        _priQueue.pop();
    }
    printf("queryword:%s\n",queryword.c_str());
    map<string,string> _queryResult = _dict->RelatedQuery(queryword);//查询结果<单词,出现次数>
    int cnt = 0;//候选词个数
    for(auto it : _queryResult){
        _relatedResult._word = it.first;
        _relatedResult._freq = atoi(it.second.c_str());
        _relatedResult._distance = distance(queryword,it.first);
        _priQueue.push(_relatedResult);
    }
    while(!_priQueue.empty() && cnt < 5){
        top5words.push_back(_priQueue.top()._word);
        _priQueue.pop();
        ++cnt;
    }
    return top5words;
}

int WordRecommander::distance(const string &lhs,const string &rhs){
    WordDistance _distance;
    return _distance.editDistance(lhs,rhs);
}