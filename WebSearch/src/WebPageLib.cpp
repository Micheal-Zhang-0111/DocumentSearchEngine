#include<iostream>
#include<fstream>
#include<regex>
#include<math.h>
#include<string.h>
#include<dirent.h>
#include<sw/redis++/redis++.h>
#include"../include/WebPageLib.h"
#include"../include/tinyxml2/tinyxml2.h"
#include"../include/simhash/Simhasher.hpp"

using std::ofstream;
using std::cout;
using std::endl;
using std::cerr;
using std::regex;
using std::to_string;
using namespace tinyxml2;
using namespace sw::redis;
using namespace simhash;


//去除字符串中所有尖括号及其内部的内容
string regexNode(const string &rhs){
    string pattern("<.*?>");
    regex r(pattern);
    return std::regex_replace(rhs,r,"");
}

/***********************************************/
WebPage::WebPage()
:_pageLib(new map<int,pair<string,bool>>)
{
    Redis2Memory();
}

WebPage::WebPage(string xmlDirPath)
:_xmlDirPath(xmlDirPath),
_pageLib(new map<int,pair<string,bool>>)
{

}

//将redis中的网页库和倒排索引表存储到内存中
void WebPage::Redis2Memory(){
    MyRedis redis1;
    redis1.connect("127.0.0.1",6379);
    redis1.select("13");
    vector<string> allKeys = redis1.getAllKeys();
    for(auto &key : allKeys){
        string value = redis1.getValue(key);
        _pageLib->insert(std::make_pair(atoi(key.c_str()),std::make_pair(value,false)));
    }
    printf("redis --> _pageLib end\n");

    vector<string> docids;
    Redis redis2("tcp://127.0.0.1:6379/14");
    allKeys.clear();
    redis2.keys("*",back_inserter(allKeys));
    for(auto &key : allKeys){
        docids.clear();
        redis2.zrange(key,0,-1,back_inserter(docids));
        for(auto &docid : docids){
            auto weight = redis2.zscore(key,docid);
            // printf("key = %s,docid = %s,  weight = %f\n",key.c_str(),docid.c_str(),*weight);
            InvertIndexTable[key].push_back(std::make_pair(atoi(docid.c_str()), *weight));
        }
    }
    printf("redis --> InvertIndexTable end\n");
}

void WebPage::getxmlPath(){
    DIR *pdir = opendir(_xmlDirPath.c_str());
    if(pdir == nullptr){
        perror("opendir");
        return;
    }
    struct dirent *pdirent;
    while((pdirent = readdir(pdir)) != nullptr){
        //DT_REG表示普通文件
        if(pdirent->d_type == DT_REG){
            string filepath = _xmlDirPath + pdirent->d_name;
            _xmlPathList.push_back(filepath);
        }
    }
    sort(_xmlPathList.begin(),_xmlPathList.end());
    //打印路径
    for(auto &path : _xmlPathList){
        cout << path << endl;
    }
    closedir(pdir);
}

//建立网页库
void WebPage::buildPageLib(){
    int docid = 1;
    for(auto xmlFilepath : _xmlPathList){
        XMLDocument doc;
        if(doc.LoadFile(xmlFilepath.c_str()) != 0){
            printf("load file error\n");
            return;
        }
        XMLElement *root = doc.RootElement();
        XMLElement *channel = root->FirstChildElement("channel");
        XMLElement *item = channel->FirstChildElement("item");
        while(item != nullptr){
            XMLElement *title = item->FirstChildElement("title");
            XMLElement *link = item->FirstChildElement("link");
            XMLElement * content = item->FirstChildElement("description");
            RssPage page;
            page._title = title->GetText();
            page._link = link->GetText();
            if(content != nullptr){
                page._content = regexNode(content->GetText());
            }else{
                page._content = "";
            }
            string fmtText = "<doc><docid>" + to_string(docid) + "</docid>" +
                             "<link>" + page._link + "</link>" +
                             "<title>" + page._title + "</title>" +
                             "<content>" + page._content + "</content></doc>";
            // _pageLib->push_back(std::make_pair(docid,fmtText),0);
            _pageLib->insert(std::make_pair(docid,std::make_pair(fmtText,false)));
            ++docid;
            item = item->NextSiblingElement();         
        }
    }
}

//建立网页偏移库
void WebPage::buildPageOffsetLib(){
    int docid = 1;//文章号
    int offset = 0;//当前指针位置(偏移量)
    for(auto page : *_pageLib){
        if(page.second.second == false){//flag = false说明不是重复文章 
            //<第几个文件，文件起始位置，文件的长度>
            int docSize = page.second.first.size();
            _pageOffsetLib.insert(std::make_pair(docid,std::make_pair(offset,docSize)));
            offset += docSize;
        }
        ++docid;
    }
}

//删除重复网页,将重复文章flag标记为1
void WebPage::DelDuplicatePages(){
    Simhasher simhasher("../include/simhash/dict/jieba.dict.utf8", 
                        "../include/simhash/dict/hmm_model.utf8", 
                        "../include/simhash/dict/idf.utf8", 
                        "../include/simhash/dict/stop_words.utf8");
    vector<uint64_t> simhashList;//相似度哈希值列表
    //网页库<文章内容,0>//flag为1表示重复元素
    int docid = 0;//文章号id
    int NoequalNum = 0;//非相似文章数
    size_t topN = 300;//提取的关键词数量
    uint64_t similarHash = 0;//相似度哈希值
    for(auto &page : *_pageLib){
        docid += 1;
        vector<pair<string,double>> keywordWeight;//提取的关键词及其权重
        simhasher.extract(page.second.first,keywordWeight,topN);
        simhasher.make(page.second.first,topN,similarHash);//计算相似度哈希值
        bool isequal = false;
        for(auto &hash : simhashList){
            if(Simhasher::isEqual(similarHash,hash)){
                isequal = true;
                page.second.second = true;
                break;
            }
        }
        if(isequal == false){//如果不是重复文章
            NoequalNum += 1;
            simhashList.push_back(similarHash);
            double weight = 0;//权重
            for(auto item : keywordWeight){
                weight += item.second * item.second;
            }
            weight = sqrt(weight);
            for(auto &elem : keywordWeight){
                InvertIndexTable[elem.first].push_back(std::make_pair(docid,elem.second/weight));
            }
        }
    }
    cout<<"NoequalNum = "<<NoequalNum<<endl;
}

// //建立倒排索引表
// void WebPage::buildInvertIndexTable(){

// }

//将网页库、网页偏移库和倒排索引表存储到文件中
void WebPage::storeLib2File(){
    //将网页库存储到文件中
    ofstream ofs("../dataLib/pageLib.dat");
    if(!ofs){
        cerr << "ofstream open error" << endl;
        return;
    }
    for(auto page : *_pageLib){
        if(page.second.second == false)
            ofs << page.second.first;
    }
    ofs.close();

    //将网页偏移库存储到文件中
    ofstream ofs2("../dataLib/pageOffsetLib.dat");
    if(!ofs2){
        cerr << "ofstream open error" << endl;
        return;
    }
    for(auto &page : _pageOffsetLib){
        ofs2 << page.first << " " << page.second.first << " " << page.second.second << endl;
    }
    ofs2.close();

    //将倒排索引表存储到文件中
    ofstream ofs3("../dataLib/InvertIndexTable.dat");
    if(!ofs3){
        cerr << "ofstream open error" << endl;
        return;
    }
    for(auto &item : InvertIndexTable){
        ofs3 << item.first <<" ";//关键词
        for(auto it : item.second){
            ofs3 << it.first << " " << it.second << " ";//文档id,词语权重
        }
        ofs3 << endl;
    }
    ofs3.close();
}

//将网页库和倒排索引表存储到redis中
void WebPage::storeLib2Redis(){
    //将网页库存储到redis 13
    // MyRedis redis;
    // _redis = redis;
    auto redis1 = Redis("tcp://127.0.0.1:6379/13");
    for(auto page : *_pageLib){
        if(page.second.second == false){
            redis1.set(to_string(page.first),page.second.first);
        }
    }
    //将倒排索引表存储到redis 14
    auto redis2 = Redis("tcp://127.0.0.1:6379/14");
    for(auto &item : InvertIndexTable){
        string key = item.first;
        for(auto it : item.second){
            // string value = to_string(it.first) + " " + to_string(it.second);
            // redis2.sadd(key,value);
            redis2.zadd(key,to_string(it.first),it.second);
        }
    }
}

/*
    计算关键词的权重
    keywordList:关键词列表
    docidList:关键词交集的 文章id列表
    docidWeightList:文章id对应的 关键词权重列表
*/
void WebPage::calDocidWordWeight(const vector<string> &keywordList,vector<string> &docidList, vector<vector<double>> &docidWeightList){
    auto redis = Redis("tcp://127.0.0.1:6379/14");
    //求keywordList中词的交集，将结果存入tmpIntersection中
    redis.zinterstore("tmpIntersection",keywordList.begin(),keywordList.end());
    //将docid按照权重从大到小顺序放入docidList中
    redis.zrange("tmpIntersection",0,-1,back_inserter(docidList));
    for(auto &docid : docidList){
        vector<double> docidWeight;
        //获取每个关键词中，文章docid所占的权重
        for(auto &keyword : keywordList){
            auto weight = redis.zscore(keyword,docid);
            docidWeight.push_back(*weight);   //******为什么是 */
        }
        docidWeightList.push_back(docidWeight);
    }
    //删除临时集合
    redis.del("tmpIntersection");
}

//将xml文件转换为struct RssPage格式,index为索引,不是docid
void WebPage::xml2Page(RssPage &page, int index){
    //redis 13 = <docid,文章内容>
    auto redis = Redis("tcp://127.0.0.1:6379/13");
    auto str = redis.get(std::to_string(index));
    string s = *str;

    regex regex1("(<link>)(.*)(</link>)");
    std::smatch result1;
    std::regex_search(s,result1,regex1);
    page._link =result1[2];

    regex regex2("(<title>)(.*)(</title>)");
    std::smatch result2;
    std::regex_search(s,result2,regex2);
    page._title =result2[2];

    regex regex3("(<content>)(.*)(</content>)");
    std::smatch result3;
    std::regex_search(s,result3,regex3);
    page._content =result3[2];
}

//根据关键词查找相似网页
//计算输入的关键词与一组文档的相似度，并将相似度最高的文档结果存储在pageList向量中
void WebPage::wordSimPage(string word,vector<RssPage> &pageList){
    set<string> keyword_set;

    Simhasher simhasher("../include/simhash/dict/jieba.dict.utf8",
                        "../include/simhash/dict/hmm_model.utf8",
                        "../include/simhash/dict/idf.utf8",
                        "../include/simhash/dict/stop_words.utf8");
    size_t topN = 3; //词频最高的前N个单词
    vector<string> keyword;
    vector<double> keyweight;
    vector<pair<string,double>> keyword_weight; //装<关键字,权重>的数组
    simhasher.extract(word,keyword_weight,topN);
    // //词关联表 <keyword,<docid,weight>>
    // map<string,vector<string,double>> wordCorrelateTable;

    vector<string> docidList;
    vector<vector<double>> docidWeightList;

    for(auto &elem : keyword_weight){
        keyword.push_back(elem.first);
        keyweight.push_back(elem.second);
        keyword_set.insert(elem.first);
    }

    calDocidWordWeight(keyword,docidList,docidWeightList);
    vector<pair<string,double>> docidWeightRes;
    for(int i = 0; i < docidWeightList.size(); ++i){
        double xy = 0, x = 0, y = 0;
        for(int j = 0; j < keyweight.size(); ++j){
            xy += docidWeightList[i][j] * keyweight[j];
            x += docidWeightList[i][j] * docidWeightList[i][j];
            y += keyweight[j] * keyweight[j];
        }
        double tmp = xy / (sqrt(x) * sqrt(y));
        docidWeightRes.push_back(std::make_pair(docidList[i], tmp));
    }
    sort(docidWeightRes.begin(),docidWeightRes.end(),[](pair<string,double> &a,pair<string,double> &b){
        return a.second > b.second;
    });

    int numAdd = 0;
    for(auto &elem : docidWeightRes){
        set<string> pageKeyword = keyword_set;
        RssPage finalPage;
        xml2Page(finalPage,atoi(elem.first.c_str()));

        size_t topN2 = 5;
        vector<pair<string,double>> FinalPage_keywordWeight;
        simhasher.extract(finalPage._content,FinalPage_keywordWeight,topN2);
        for(auto &keyWeight: FinalPage_keywordWeight){
            pageKeyword.insert(keyWeight.first);
        }
        for(auto &key : pageKeyword){
            finalPage._keyword += key + " ";
        }
        pageList.push_back(finalPage);
        ++numAdd;
        if(numAdd > 5){
            break;
        }
    }

}


/* 1 */
/*	simhasher.extract(_words,res,topN);
    功能：
*		抽取关键字，内部调用了_extractor.extract();
*	参数：
*		(1)text	是传入的将要进行抽取关键字的字符串
*		(2)res	装“关键字/权重”的数组
*		(3)topN	词频最高的前N个单词
*/