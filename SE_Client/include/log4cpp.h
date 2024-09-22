//编译时加上-llog4cpp -lpthread
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

using std::cout;
using std::ostream;
using std::ostringstream;
using std::string;

/**
 * @brief 日志记录器类，用于记录日志信息。
 */
class Mylogger
{
public:
    static Mylogger &getInstance(const string &filename = "../log/SearchEngine.log",const string &fileAppenderName = "logfile",
                    ostream &os = cout,const string &osAppenderName = "teminal")
    {
        static Mylogger log(filename,fileAppenderName,os,osAppenderName);
        return log;
    }
    template<typename... Args>
    void info(const string &msg,const Args &... args){
        root.info(msg.c_str(),args...);
    }
    template<typename... Args>
    void error(const string &msg,const Args &... args){
        root.error(msg.c_str(),args...);
    }
    template<typename... Args>
    void debug(const string &msg,const Args &... args){
        root.debug(msg.c_str(),args...);
    }
    template<typename... Args>
    void warn(const string &msg,const Args &... args){
        root.warn(msg.c_str(),args...);
    }


private:
    Mylogger() = default;
    Mylogger(const string &filename,const string &fileAppenderName,ostream &os,const string &osAppenderName){
        root.setPriority(log4cpp::Priority::DEBUG);
        log4cpp::OstreamAppender *osAppender = new log4cpp::OstreamAppender(osAppenderName,&os);
        log4cpp::FileAppender *fileAppender = new log4cpp::FileAppender(fileAppenderName,filename);
        log4cpp::PatternLayout *pLayout1 = new log4cpp::PatternLayout;
        pLayout1->setConversionPattern("%d %c [%p] %m%n");
        log4cpp::PatternLayout *pLayout2 = new log4cpp::PatternLayout;
        pLayout2->setConversionPattern("%d %c [%p] %m%n");
        osAppender->setLayout(pLayout1);
        fileAppender->setLayout(pLayout2);
        //不能共用一个Layout,不然会报错，每个appender应该使用独立的layout
        root.addAppender(osAppender);
        root.addAppender(fileAppender);
    }
    log4cpp::Category &root = log4cpp::Category::getRoot();
};

#define FORMATMSG(msg) string(__FILE__).append("(")\
                        .append(std::to_string(__LINE__)).append(")")\
                        .append("-<").append(__FUNCTION__)\
                        .append(">: ").append(msg)
#define logInfo(msg, ...) Mylogger::getInstance().info(FORMATMSG(msg), ##__VA_ARGS__)
#define logError(msg, ...) Mylogger::getInstance().error(FORMATMSG(msg), ##__VA_ARGS__)
#define logDebug(msg, ...) Mylogger::getInstance().debug(FORMATMSG(msg), ##__VA_ARGS__)
#define logWarn(msg, ...) Mylogger::getInstance().warn(FORMATMSG(msg), ##__VA_ARGS__)
