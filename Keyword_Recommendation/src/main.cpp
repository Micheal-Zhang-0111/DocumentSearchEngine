#include"../include/CppJieba.h"
#include"../include/DictProducer.h"
#include"../include/WordRecommand.h"

void testshow(vector<string> words){
    for(auto &e : words){
        std::cout << e << std::endl;
    }
}

void test(){
    CppJieba jieba;
    string s = "我爱北京天安门";
    vector<string> words = jieba.cut(s);
    testshow(words);

    EnJieba enjieba;
    string s1 = "I love Beijing Tiananmen";
    vector<string> words1 = enjieba.cut(s1);
    testshow(words1);
}

void test2(){
    string queryword;
    WordRecommander recommander;
    while(1){
        printf("请输入查询词:");
        std::cin >> queryword;
        cout << "推荐词\t\t词频\t\t编辑距离" << endl;
        recommander.doQuery(queryword);
        cout<<endl;
    }
}


int main(){
    test2();
    return 0;
}