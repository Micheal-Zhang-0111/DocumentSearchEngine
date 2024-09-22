#include"../../include/SearchEngineHpp/MyRedis.h"

//用于连接到Redis数据库
bool MyRedis::connect(string host,int port){
    _pConnect = redisConnect(host.c_str(),port);
    if(_pConnect == nullptr || _pConnect->err){
        if(_pConnect){
            cout<<"connect error:"<<_pConnect->errstr<<endl;
        }else{
            cout<<"connect error:can't allocate redis context"<<endl;
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

//设置一个键值对到Redis数据库中
void MyRedis::set(string key,string value){
    _pReply = (redisReply*)redisCommand(_pConnect,"SET %s %s",key.c_str(),value.c_str());
    freeReplyObject(_pReply);
    _pReply = nullptr;
}

//将一个值添加到Redis数据库中的一个集合中
void MyRedis::sadd(string key,string value){
    _pReply = (redisReply*)redisCommand(_pConnect,"SADD %s %s",key.c_str(),value.c_str());
    freeReplyObject(_pReply);
    _pReply = nullptr;
}

//获取Redis数据库中一个集合的所有成员
vector<string> MyRedis::smembers(string key){
    _pReply = (redisReply*)redisCommand(_pConnect,"SMEMBERS %s",key.c_str());
    vector<string> values;
    for(int i = 0;i < _pReply->elements;++i){
        values.push_back(_pReply->element[i]->str);
    }
    freeReplyObject(_pReply);
    _pReply = nullptr;
    return values;
}

//选择Redis数据库中的一个数据库
void MyRedis::select(string num){
    _pReply = (redisReply*)redisCommand(_pConnect,"SELECT %s",num.c_str());
    freeReplyObject(_pReply);
    _pReply = nullptr;
}

//获取两个集合的交集
std::set<string> MyRedis::sinter(string key1,string key2){
    _pReply = (redisReply*)redisCommand(_pConnect,"SINTER %s %s",key1.c_str(),key2.c_str());
    std::set<string> values;
    for(int i = 0;i < _pReply->elements;++i){
        values.insert(_pReply->element[i]->str);
    }
    freeReplyObject(_pReply);
    _pReply = nullptr;
    return values;
}
