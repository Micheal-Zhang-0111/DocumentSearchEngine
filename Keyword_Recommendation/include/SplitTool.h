#pragma once

#include<string>
#include<vector>
using std::string;
using std::vector;

class SplitTool
{
public:
    virtual vector<string> cut(string &sentence) = 0;

    virtual ~SplitTool() {}
};