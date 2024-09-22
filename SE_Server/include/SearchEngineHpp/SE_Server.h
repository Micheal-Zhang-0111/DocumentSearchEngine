#pragma once

#include<iostream>
#include<unistd.h>
#include<string.h>
#include"../SearchEngineHpp/log4cpp.h"
#include"../SearchEngineHpp/SE_WordRecommand.h"
#include"../SearchEngineHpp/SE_WebPageQuery.h"
#include"../ThreadPool/TcpServer.h"
#include"../ThreadPool/ThreadPool.h"
#include"../ThreadPool/JsonTransform.h"

using std::cout;
using std::endl;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    :_msg(msg),
    _con(con),
    _wordRecommander(),
    _webPageSearch("../xmlDir")
    {
    }

    void process(){
        //在process函数中去进行真正的业务逻辑的处理
        //cout<<"请输入类型：1.关键字推荐 2.网页库查询"<<endl;
        //拆分
        string k_w_n_Msg;//实际传输 key or web or nothing
        char c_num[100]="";//存"1."
        char c_real_msg[100]="";//存"hello"

        char c_msg[1024]="";//存"1.hello"
        strcpy(c_msg,_msg.c_str());//将_msg转成c串

        int i=0;
        int j=0;
        string num;
        string real_msg;
        json jTop5;
        json jPageList = json::array();
        json jPage;
        while(c_msg[i]!='.')
        {
            if(c_msg[i]=='\n'){
                goto wrongplace;
            }
            c_num[i]=c_msg[i];
            i++;
        }
        i=i+1;
        while(c_msg[i]!='\n')
        {
            c_real_msg[j]=c_msg[i];
            i++;
            j++;
        }
        num = c_num;//数字"1."
        real_msg = c_real_msg;//真正内容如："hello"

        if (num.compare("1") == 0)
        {
            logInfo("关键词推荐：%s",c_real_msg);
            k_w_n_Msg=(_wordRecommander.doQuery(real_msg));
            vector<string> top5words = _wordRecommander.Top5Query(real_msg);
            for(int i = 0; i < top5words.size(); i++){
                string index = to_string(i);
                jTop5[index]=top5words[i];
                // printf("top5words = %s\n",top5words[i].c_str());
            }
            _con->sendInLoop(jTop5.dump());
        }
        else if (num.compare("2") == 0)
        {
            logInfo("网页查询：%s",c_real_msg);
            vector<RssPage> pages;
            _webPageSearch.wordSimPage(real_msg,pages); //结果全部存在pages中（vector）
            for (auto &elem : pages)
            {
                jPage["title"] = elem._title;
                jPage["link"] = elem._link;
                jPage["keywords"] = elem._keyword;
                jPageList.push_back(jPage);
                k_w_n_Msg+="\n";
                k_w_n_Msg+="标题:";k_w_n_Msg+=elem._title;k_w_n_Msg+="\n";
                k_w_n_Msg+="关键词:";k_w_n_Msg+=elem._keyword;k_w_n_Msg+="\n";
                k_w_n_Msg+="链接:";k_w_n_Msg+=elem._link;k_w_n_Msg+="\n";
            }
            printf("k_w_n_Msg = %s\n",k_w_n_Msg.c_str());
            _con->sendInLoop(jPageList.dump());
        }
        else
        {
        wrongplace:
            k_w_n_Msg = "请选择正确选项\n";
            logDebug("请选择正确选项：%s",c_real_msg);
        }

        // _con->sendInLoop(k_w_n_Msg);
        cout<<"查询完毕"<<endl;
    }

private:
    string _msg;
    TcpConnectionPtr _con;
    WordRecommander _wordRecommander;
    WebPage _webPageSearch;
};


class SearchEngineServer
{
public:
    //线程数，队列大小，ip，端口
    SearchEngineServer(size_t threadNum, size_t queSize,
                        const string &ip, unsigned short port)
    :_threadpool(threadNum,queSize),
    _server(ip,port)
    {
    }

    void start(){
        _threadpool.start();
        using namespace std::placeholders;
        _server.setAllCallback(std::bind(&SearchEngineServer::onConnection,this,_1),
                                std::bind(&SearchEngineServer::onMessage,this,_1),
                                std::bind(&SearchEngineServer::onClose,this,_1));
        _server.start();
    }

    void stop(){
        _threadpool.stop();
        _server.stop();
    }

    void onConnection(const TcpConnectionPtr &con){
        cout<<con->toString()<<" has connected!"<<endl;
    }

    void onMessage(const TcpConnectionPtr &con){
        json recvjMsg = json::parse(con->receive().c_str());
        Data dMsg;
        bzero(&dMsg,sizeof(dMsg));
        jsonTransform jTrans(dMsg);//dMsg->jTrans._data转换
        jTrans.from_json(recvjMsg);//处理过的json包，数据在jTrans._data
        cout<<"查询中..."<<endl;
        MyTask task1(jTrans.getData(),con);
        _threadpool.addTask(std::bind(&MyTask::process,task1));
    }

    void onClose(const TcpConnectionPtr &con){
        cout<<con->toString()<<" has closed!"<<endl;
    }

private:
    ThreadPool _threadpool;
    TcpServer _server;
};