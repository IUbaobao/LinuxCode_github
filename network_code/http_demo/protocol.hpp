#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Util.hpp"

const std::string sep="\r\n";
const std::string default_root="./wwwroot";
const std::string home_page="index.html";

class HttpRequest
{
public:
    void parse()
    {
        std::string line=Util::getOneline(inbuffer,sep);
        if(line.empty()) return;

        std::stringstream ss(line);
        ss>>method>>url>>httpversion;

        //添加web默认路径
        path=default_root;//./wwwroot
        path+=url;// ./wwwroot/
        if(path[path.size()-1]=='/') path+=home_page;// ./wwwroot/index.html;


        // 获取path对应的资源后缀
        // ./wwwroot/index.html
        // ./wwwroot/test/a.html
        // ./wwwroot/image/1.jpg
        auto pos=path.rfind(".");
        if(pos==std::string::npos)
            suffix=".html";
        else
            suffix=path.substr(pos);

        //得到资源大小
        struct stat st;
        int n=stat(path.c_str(),&st);
        if(n==0) size=st.st_size;
        else size=-1;
    }
public:
    std::string inbuffer;
    std::string method ;
    std::string url;
    std::string httpversion;
    std::string path;
    std::string suffix;
    int size;
};



class HttpResponse
{
public:
    std::string outbuffer;
};

