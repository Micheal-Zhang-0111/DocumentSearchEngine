#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<regex>
#include"../include/tinyxml2.h"

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::vector;
using std::regex;

using namespace tinyxml2;

struct RSSItem{
    string _title;
    string _link;
    string _description;
    string _content;
};

class RSS
{
public:
    RSS(size_t capacity);
    ~RSS() = default;

    //读文件
    void read(const string &filename);
    //存储文件
    void store(const string &filename);


private:
    vector<RSSItem> _rssList;
};