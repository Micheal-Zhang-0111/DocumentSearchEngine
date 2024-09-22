#include"../../include/SearchEngineHpp/WordDistance.h"

WordDistance::WordDistance(){
    // printf("WordDistance()\n");
}

//判断字符是否为中文(utf-8编码)
bool WordDistance::isChinese(const string &str){
    int i = 0;
    while(str[i] != 0){
        if(str[i] & 0x80 && str[i] & 0x40 && str[i] & 0x20){
            //utf-8编码的中文字符最高3位是1
            i += 3;
        }
        else{
            return false;
        }
    }
    return true;
}

int WordDistance::editDistance(const string &lhs,const string &rhs){
    int byte;
    if(isChinese(lhs)){
        byte = 3;
    }
    else{
        byte = 1;
    }
    int len1 = lhs.size(),len2 = rhs.size();
    string word1 = " " + lhs;
    string word2 = " " + rhs;
    vector<vector<int>> dp(len1+1,vector<int>(len2+1));
    for(int i = 1; i <= len1; ++i){
        dp[i][0] = i;
    }
    for(int i = 1; i <= len2; ++i){
        dp[0][i] = i;
    }
    for(int i = 1; i <= len1; ++i){
        for(int j = 1; j <= len2; ++j){
            if(word1[i] == word2[j]){
                dp[i][j] = dp[i-1][j-1];
            }
            else{
                //删除、添加、替换
                dp[i][j] = std::min(dp[i-1][j],std::min(dp[i][j-1],dp[i-1][j-1])) + 1;
            }
        }
    }
    return dp[len1][len2]/byte;
}