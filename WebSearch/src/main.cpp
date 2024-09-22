#include"../include/WebPageLib.h"


void test(){
    WebPage page("../xmlDir/");
    page.getxmlPath();
    page.buildPageLib();
    page.buildPageOffsetLib();
    page.DelDuplicatePages();
    // page.storeLib2File();
    page.storeLib2Redis();
}   

void test2(){
    vector<string> keywordList = {"柱管","磨损"};
    vector<string> docidList;
    vector<vector<double>> docidWeightList;
    WebPage page("../xmlDir/");
    page.calDocidWordWeight(keywordList,docidList,docidWeightList);
    for(auto &docid : docidList){
        cout << docid << " ";
    }cout<<endl;
    for(auto &docidWeight : docidWeightList){
        for(auto &weight : docidWeight){
            cout << weight << " ";
        }cout<<endl;
    }
}

void test3(){
    // WebPage page("../xmlDir/");
    time_t start,stop;
    start = time(NULL);
    WebPage page("../xmlDir/");
    // WebPage page;
    // page.Redis2Memory();
    stop = time(NULL);
    printf("Redis2Memory() cost %ld seconds\n",stop-start);
    while(1){
        vector<RssPage> queryres;
        string queryword;
        std::cin >> queryword;

        page.wordSimPage(queryword,queryres);
        for(auto &elem : queryres){
            cout << elem._title << endl;
            cout << elem._link << endl;
            cout << elem._content << endl;
            cout << elem._keyword << endl;
            cout<<"=================================================="<<endl;
        }
    }
    
}

int main(){
    test3();
    return 0;
} 