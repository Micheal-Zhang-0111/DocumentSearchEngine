#include"../include/WordDistance.h"

//计算两个单词之间的编辑距离
int WordDistance::editDistance(const string &lhs,const string &rhs){
    int byte;//判断中英文
    if(_dict->isChinese(lhs)){
        byte = 3;//中文
    }
    else{//英文
        byte = 1;
    }
    int len1 = lhs.size(), len2 = rhs.size();
    string word1 = " " + lhs;
    string word2 = " " + rhs;
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
    for(int i = 1; i <= len1; ++i){
        dp[i][0] = i;
    }
    for(int i = 1; i <= len2; ++i){
        dp[0][i] = i;
    }
    for(int i = 1; i <= len1; ++i){
        for(int j = 1; j <= len2; ++j){
            if(word1[i] == word2[j]){
                dp[i][j] = dp[i - 1][j - 1];
            }
            else{
                dp[i][j] = std::min(dp[i - 1][j - 1], std::min(dp[i - 1][j], dp[i][j - 1])) + 1;
            }
        }
    }
    return dp[len1][len2]/byte;
}