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
    //创建一个套接字
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_fd == -1){
        perror("socket error");
        return;
    }
    //设置服务器地址
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //连接服务器
    if(connect(client_fd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1){
        perror("connect error");
        close(client_fd);
        return;
    }
    printf("connect success\n");

    //创建一个data对象并初始化
    Data d;
    d.length = 10;
    d.id = 1;
    d.data = "hello";
    //将data对象转换为json对象
    json j;
    Data2json(j,d);
    //将json格式序列化
    string serialized_string = j.dump();
    printf("serialized_string:%s\n",serialized_string.c_str());
    while(1){
        //发送到服务器
        int ret = send(client_fd,serialized_string.c_str(),serialized_string.size(),MSG_NOSIGNAL);
        if(ret == -1){
            perror("send error");
            exit(-1);
        }
        sleep(3);
        //接受服务器返回的数据
        char recvBuf[1024] = {0};
        ret = recv(client_fd,recvBuf,sizeof(recvBuf),0);
        if(ret == 0){
            perror("recv 0");
        }
        else if(ret == -1){
            perror("recv -1");
        }
        else{
            printf("recv data from server:%s\n",recvBuf);
        }
    }
}

int main(){
    test();
    return 0;
}