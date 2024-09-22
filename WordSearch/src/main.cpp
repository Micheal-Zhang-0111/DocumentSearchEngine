#include"../include/CppJieba.h"
#include"../include/WordRecommand.h"


void test5(){
    WordRecommander recommander;
    string queryword;
    while(1){
        printf("请输入查询词:");
        std::cin >> queryword;
        cout << "推荐词\t\t词频\t\t编辑距离" << endl;
        recommander.doQuery(queryword);
        cout<<endl;
    }
    
}

int main(){
    test5();
    return 0;
}