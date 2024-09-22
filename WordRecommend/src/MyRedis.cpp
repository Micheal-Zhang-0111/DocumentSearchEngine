#include"../include/MyRedis.h"

//用于连接到Redis数据库
bool MyRedis::connect(string ip,int port){
    _pConnect = redisConnect(ip.c_str(),port);
    if(_pConnect == nullptr || _pConnect->err){
        if(_pConnect){
            fprintf(stderr,"Connection error:%s\n",_pConnect->errstr);
        }
        else{
            fprintf(stderr,"Connection error:can't allocate redis context\n");
        }
        return false;
    }
    return true;
}

//获取Redis数据库中所有的键
vector<string> MyRedis::getAllKeys(){
    _pReply = (redisReply *)redisCommand(_pConnect,"KEYS *");
    printf("get keys success\n");
    vector<string> keys;
    for(int i = 0; i < _pReply->elements; ++i){
        keys.push_back(_pReply->element[i]->str);
    }
    freeReplyObject(_pReply);
    _pReply = nullptr;
    return keys;
}

//用Redis数据库中获取一个键对应的值
string MyRedis::getValue(string key){
    _pReply = (redisReply*)redisCommand(_pConnect,"GET %s",key.c_str());
    string value = _pReply->str;
    freeReplyObject(_pReply);
    _pReply = nullptr;
    return value;    
}

//向Redis数据库中插入一个键值对
void MyRedis::set(string key,string value){
    (redisReply *)redisCommand(_pConnect,"SET %s %s",key.c_str(),value.c_str());
}

//将一个值添加到Redis数据库中的一个集合中
void MyRedis::sadd(string key,string value){
    (redisReply *)redisCommand(_pConnect,"SADD %s %s",key.c_str(),value.c_str());
}

//选择Redis中的一个数据库
void MyRedis::select(string num){
    (redisReply *)redisCommand(_pConnect,"SELECT %s",num.c_str());
}

//获取Redis数据库中一个集合的所有成员
vector<string> MyRedis::smembers(string key){
    _pReply = (redisReply*)redisCommand(_pConnect,"SMEMBERS %s",key.c_str());
    vector<string> values;
    for(int i = 0; i < _pReply->elements; ++i){//elements元素数量
        values.push_back(_pReply->element[i]->str);
    }
    freeReplyObject(_pReply);
    _pReply = nullptr;
    return values;
}

//获取两个集合的交集
std::set<string> MyRedis::sinter(string key1,string key2){
    _pReply = (redisReply*)redisCommand(_pConnect,"SINTER %s %s",key1.c_str(),key2.c_str());
    std::set<string> values;//set的值唯一
    for(int i = 0;i < _pReply->elements;++i){
        values.insert(_pReply->element[i]->str);
    }
    freeReplyObject(_pReply);
    _pReply = nullptr;
    return values;
}
