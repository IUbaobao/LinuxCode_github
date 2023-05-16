#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <ctime>
#include <cstdarg>
#include <cstdio>

#define LOG_NORMAL "log.txt"
#define LOG_ERR "log.error"

#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3
#define FATAL 4

using namespace std;

#define NUM 1024
const char *to_levelstr(int level)
{
    switch (level)
    {
    case DEBUG:
        return "DEBUG";
    case NORMAL:
        return "NORMAL";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    case FATAL:
        return "FATAL";
    default:
        return "nullptr";
    }
}

void logMessage(int level, const char *format, ...)
{
    // [日志等级] [时间戳/时间] [pid] [messge]
    char buff[NUM];
    time_t nowtime;
    time(&nowtime);
    tm *p = localtime(&nowtime);
    snprintf(buff, sizeof buff, " [%s][%4d.%02d.%02d %02d:%02d:%02d] [%d] ", to_levelstr(level), p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, getpid());

    char logcontent[NUM];
    va_list arg;
    va_start(arg, format);
    vsnprintf(logcontent, sizeof(logcontent), format, arg);


    FILE* log=fopen(LOG_NORMAL,"a");
    FILE* err=fopen(LOG_ERR,"a");
    if(log!=nullptr && err !=nullptr)
    {
        FILE* cur=nullptr;
        if(level==DEBUG || level==NORMAL || level==WARNING) cur=log;
        if(level==ERROR || level==FATAL) cur=err;
        if(cur) fprintf(cur,"%s%s\n",buff,logcontent);

        fclose(log);
        fclose(err);
    }
}
