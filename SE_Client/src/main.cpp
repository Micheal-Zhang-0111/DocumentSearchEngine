#include <signal.h>
#include <iostream>
#include"../include/SE_ClientWeb.h"

void test(){
    SearchEngineClientWeb client(1);
    client.loadCssJs();
    client.loadImageSource();
    client.loadHomePage();
    client.start(1234);
}

int main(){
    test();
    return 0;
}