#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include<nlohmann/json.hpp>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using json = nlohmann::json;

//定义一个结构体存储数据
struct Data
{
    int length;//数据长度
    int id;//数据id
    string data;//数据内容
};

//将Data结构体转换为json对象
void Data2json(json &j,const Data &d){
    j = json{{"length",d.length},
             {"id",d.id},
             {"data",d.data}};
}

//将json对象转换为Data结构体
void json2Data(const json &j,Data &d){
    d.length = j.at("length");
    j.at("id").get_to(d.id);
    j.at("data").get_to(d.data);
}

void test(){
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int opt = 1;
    int ret = setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(ret == -1){
        perror("setsockopt error");
        return;
    }
    ret = setsockopt(listenFd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    if(ret == -1){
        perror("setsockopt error");
        return;
    }
    bind(listenFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    listen(listenFd,128);
    printf("listen success\n");
    int clientFd = accept(listenFd,NULL,NULL);
    printf("connect success,fd = %d\n",clientFd);
    //收发json格式的数据
    char buf[1024];

    while(1){
        memset(buf,0,sizeof(buf));
        int ret = recv(clientFd,buf,sizeof(buf),0);
        cout<<"recv client : "<<buf<<endl;
        if(ret == 0){
            perror("recv 0");
            exit(-1);
        }
        else if(ret == -1){
            perror("recv error");
            exit(-1);
        }
        Data d;
        json j1 = json::parse(buf);
        json2Data(j1,d);
        string appendword = "world";
        d.data += appendword;
        d.length += appendword.size();

        //回写给客户端
        json j2;
        Data2json(j2,d);
        ret = send(clientFd,j2.dump().c_str(),j2.dump().size(),0);
    }
}

int main(){
    test();
    return 0;
}