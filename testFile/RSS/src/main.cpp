#include<iostream>
#include"../include/rss.h"


int main(){
    RSS rss(500);
    rss.read("../bin/coolshell.xml");
    rss.store("../bin/coolshell_pagelib.dat");
    return 0;
}
