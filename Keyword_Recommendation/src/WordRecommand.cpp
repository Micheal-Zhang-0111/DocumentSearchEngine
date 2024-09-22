#include"../include/WordRecommand.h"

WordRecommander::WordRecommander(){
    _dict = Dictionary::getInstance();
    _dict->initDict("../WordFreq_Idx_Dir/wordFreqDir/","../WordFreq_Idx_Dir/wordIdxDir/");
    // _dict->readRedis2Dict();
}

void WordRecommander::doQuery(const string &queryword){
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
    while(!_priQueue.empty() && cnt < 10){
        printf("%s \t\t %d \t\t %d\n",_priQueue.top()._word.c_str(),_priQueue.top()._freq,_priQueue.top()._distance);
        _priQueue.pop();
        ++cnt;
    }
}

int WordRecommander::distance(const string &lhs,const string &rhs){
    WordDistance _distance;
    return _distance.editDistance(lhs,rhs);
}