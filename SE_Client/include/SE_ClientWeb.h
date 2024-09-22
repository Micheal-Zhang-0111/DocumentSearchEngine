#pragma once

#include <workflow/WFFacilities.h>
#include <wfrest/HttpServer.h>

class SearchEngineClientWeb
{
public:
    SearchEngineClientWeb(int nleft);
    ~SearchEngineClientWeb() = default;

    void loadCssJs();//加载网页资源
    void loadImageSource();//加载网页资源
    void loadHomePage();

    void start(unsigned short port);

private:
    wfrest::HttpServer _server;
    WFFacilities::WaitGroup _waitGroup;
};