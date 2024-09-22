#include<chrono>
// #include"../../include/SearchEngineHpp/SE_WordRecommand.h"
#include"../../include/SearchEngineHpp/SE_Server.h"

void test2(){
    string queryword;
    WordRecommander recommander;
    while(1){
        printf("请输入查询词:");
        std::cin >> queryword;
        cout << "推荐词\t\t词频\t\t编辑距离" << endl;
        auto start = std::chrono::high_resolution_clock::now();
        recommander.doQuery(queryword);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        cout<<"查询耗时:"<<duration<<"ms"<<endl;
        cout<<endl;
    }
}

int main(){
    SearchEngineServer server(5,10,"127.0.0.1",8888);
    server.start();
    return 0;
}