#include"../include/tinyxml2.h"
#include"../include/rss.h"


RSS::RSS(size_t capacity = 500)
{
    _rssList.reserve(capacity);
}

//读文件
void RSS::read(const string &filename){
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    if(doc.ErrorID()){
        std::cerr<<"loadFile fail"<<endl;
        return;
    }

    //获取第一个rss元素的第一个<channel>元素的第一个<item>元素
    XMLElement *itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    while(itemNode)
    {
        //获取<item>元素下的<title>,<link>,<description>和<content:encoded>元素的文本内容
        string title = itemNode->FirstChildElement("title")->GetText();
        string link = itemNode->FirstChildElement("link")->GetText();
        string description = itemNode->FirstChildElement("description")->GetText();
        string content = itemNode->FirstChildElement("content:encoded")->GetText();

        //正则表达式匹配
        std::regex reg("<[^>]+>");
        description = regex_replace(description, reg, "");//使用正则表达式替换<description>中的尖括号标签及其内容为空字符串
        content = regex_replace(content, reg, "");//使用正则表达式替换<content:encoded>中的尖括号标签及其内容为空字符串

        RSSItem rssItem;
        rssItem._title = title;
        rssItem._link = link;
        rssItem._description = description;
        rssItem._content = content;
        //添加到vector中
        _rssList.push_back(rssItem);
        //遍历下一个<item>元素
        itemNode = itemNode->NextSiblingElement("item");
    }
}

//存储文件
void RSS::store(const string &filename){
    ofstream ofs(filename);
    if(!ofs){
        std::cerr << "open "<< filename << "fail" << endl;
        return;
    }
    //遍历vector中的每一个元素，将其存储到文件中
    for(size_t index = 0; index != _rssList.size(); ++index){
        ofs << "<doc><docid>" << index + 1 << "</docid>"
            << "<title>" << _rssList[index]._title << "</title>"
            << "<link>" << _rssList[index]._link << "</link>"
            << "<description>" << _rssList[index]._description << "</description>"
            << "<content>" << _rssList[index]._content << "</content></doc>";
        ofs << '\n';
    }
    ofs.close();
}