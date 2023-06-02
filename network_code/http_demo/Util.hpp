#pragma once 


#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>


class Util
{
public:
    static std::string getOneline(std::string& inbuffer,const std::string& sep)
    {
        auto pos=inbuffer.find(sep);
        if(pos==std::string::npos) return "";
        std::string sub=inbuffer.substr(0,pos);
        inbuffer.erase(0,sub.size()+sep.size());
        return sub;
    }


    static bool readFile(const std::string& resource,char* buffer,size_t size)
    {
        int fd=open(resource.c_str(),O_RDONLY);

        if(fd<0) return false;
        size_t n=read(fd,buffer,size);
        if(n<0) return false;
        close(fd);
        return true;
    }
};