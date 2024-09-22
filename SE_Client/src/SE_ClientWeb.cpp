#include <iostream>

#include <workflow/WFTaskFactory.h>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <nlohmann/json.hpp>
#include<string>

#include"../include/SE_ClientWeb.h"
#include"../include/ClientConnect.h"

using namespace wfrest;
using std::string;
using Json = nlohmann::json;

const string MYSQL_USER("root");
const string MYSQL_PASSWORD("root");
const string MYSQL_HOST("localhost");

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

//将前端数据解码（解决中文乱码问题）
std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]); 
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}


SearchEngineClientWeb::SearchEngineClientWeb(int nleft)
:_server(),
_waitGroup(nleft)
{
}

void SearchEngineClientWeb::loadCssJs(){
    _server.GET(
        "/css/baidu.css", 
        [](const HttpReq *, HttpResp *resp) 
        {
            resp->File("../frontend/css/baidu.css"); 
        });
    _server.GET(
        "/js/baiduSearch.js", 
        [](const HttpReq *, HttpResp *resp) 
        {
            resp->File("../frontend/js/baiduSearch.js"); 
        });
    //newsPage的CSS
    _server.GET(
        "/home/css/newsPageCss/newsPage.css", 
        [](const HttpReq *, HttpResp *resp) 
        {
            resp->File("../frontend/css/newsPageCss/newsPage.css"); 
        });
}

void SearchEngineClientWeb::loadImageSource(){
    _server.GET(
        "/images/wangpan.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/wangpan.png");
        });
    _server.GET(
        "/images/zhidao.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/zhidao.png");
        });
    _server.GET(
        "/images/baike.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/baike.png");
        });
    _server.GET(
        "/images/tu.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/tu.png");
        });
    _server.GET(
        "/images/baobaozhidao.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/baobaozhidao.png");
        });
    _server.GET(
        "/images/wenku.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/wenku.png");
        });
    _server.GET(
        "/images/jingyan.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/jingyan.png");
        });
    _server.GET(
        "/images/yinyue.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/yinyue.png");
        });
    _server.GET(
        "/images/baidu_logo.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/baidu_logo.png");
        });
    _server.GET(
        "/images/1.jpg", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/1.jpg");
        });
    _server.GET(
        "/images/2.jpg", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/2.jpg");
        });
    _server.GET(
        "/images/searchfont.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/searchfont.png");
        });
    _server.GET(
        "/images/bg.jpg", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/bg.jpg");
        });

    //newsPage的图片资源
    _server.GET(
        "/home/images/musicalNotes.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/musicalNotes.png");
        });
    _server.GET(
        "/home/images/website.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/website.png");
        });
    _server.GET(
        "/home/images/qq.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/qq.png");
        });
    _server.GET(
        "/home/images/mail.png", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/mail.png");
        });
    //newsPage的背景图片资源
    _server.GET(
        "/home/images/scene.jpg", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/images/scene.jpg");
        });
    //newsPage的mp3资源
    _server.GET(
        "/home/music/TheRoadNotTaken.mp3", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/music/TheRoadNotTaken.mp3");
        });
    //newsPage的JS资源
    _server.GET(
        "/home/js/newsPageJs/newsPage.js", 
        [](const HttpReq *, HttpResp *resp)
        {
            resp->File("../frontend/js/newsPageJs/newsPage.js");
        });  
}


void SearchEngineClientWeb::loadHomePage(){
    // string mysqlURL = "mysql://" + MYSQL_USER + ":" + MYSQL_PASSWORD + "@" + MYSQL_HOST;
    _server.GET(
        "/home", 
        [](const HttpReq *, HttpResp *resp) 
        {
            resp->File("../frontend/baidu.html"); 
        });
    _server.GET(
        "/home/newsPage", 
        [](const HttpReq *, HttpResp *resp) 
        {
            resp->File("../frontend/newsPage.html");
            
        });
    _server.POST(
        "/home/newsPage", 
        [](const HttpReq *, HttpResp *resp) 
        {
            
        });

    _server.POST(
        "/home",
        [](const HttpReq *req, HttpResp *resp, SeriesWork *series)
        {
            if (req->content_type() != APPLICATION_URLENCODED)
            {
                resp->String("REQUEST ERROR");
                return;
            }

            //接收前端输入框传来的数据
            auto formKV = req->form_kv();
            string searchContent = formKV["searchContent"];
            // searchContent = searchContent.substr(2);
            searchContent = UrlDecode(searchContent);
            printf("searchContent: %s\n", searchContent.c_str());
            //将搜索内容传给搜索引擎
            ClientConnect client("127.0.0.1",8888);
            string msg = searchContent;
            if (strcmp(msg.c_str(), "clear") == 0)
            {
                system("clear");
            }
            else if (strcmp(msg.c_str(), "help") == 0)
            {
                puts("._______________________.");
                puts("|\033[1;35m====操作帮助文档集====\033[m |");
                
                puts("|_______________________|");
            }
            char c_msg[1024]="";
            strcpy(c_msg,msg.c_str());
            c_msg[strlen(msg.c_str())]='\n';//将传输的数据末尾加上\n，来适应服务端
            string newMsg(c_msg);

            //打包成json
            Data dMsg;
            dMsg.length=newMsg.size();
            dMsg.data=newMsg;
            jsonTrans jTrans(dMsg);//dMsg->jTrans._data
            
            //发送json
            client.sendMessage(jTrans);//真正发送的是字符串
            //接收json
            json retjMsg = client.recvMessage();
            
            //清空json并接收
            bzero(&dMsg,sizeof(dMsg));
            jTrans.from_json(retjMsg);
            // printf("printData()\n");
            // jTrans.printData();
            resp->String(jTrans.getData());
            printf("%s\n",jTrans.getData().c_str());
        }
    );
}


void SearchEngineClientWeb::start(unsigned short port){
    if (_server.track().start(port) == 0)
    {
        _server.list_routes();
        fprintf(stderr, "server start success\n");
        _waitGroup.wait();
        return;
    }
    printf("server start failed\n");
}
