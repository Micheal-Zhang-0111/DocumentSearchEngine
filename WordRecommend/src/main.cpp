
#include<chrono>
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
        auto start = std::chrono::high_resolution_clock::now();
        recommander.doQuery(queryword);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        cout<<"查询耗时:"<<duration<<"ms"<<endl;
        cout<<endl;
    }
}

void test3(){
    CppJieba *cn = new CppJieba;
    EnJieba *en = new EnJieba;
    DictProducer dict1(cn);
    dict1.buildCnDict();
    DictProducer dict2(en);
    dict2.buildEnDict();
}

void test4(){
    Dictionary *dict = Dictionary::getInstance();
    dict->storeFile2Redis("../WordFreq_Idx_Dir/wordFreqDir/","../WordFreq_Idx_Dir/relatedWordDir/");
}

int main(){
    test2();
    return 0;
}