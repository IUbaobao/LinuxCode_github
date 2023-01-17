#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NAMED_PIPE "/tmp/mypipe"

bool createFifo(const std::string& path)
{
    int n= mkfifo(path.c_str(),0666);
    if(n==0)
        return true;
    else
    {
        std::cout<<"errno:"<<errno<<"\nerrno string:"<<strerror(errno)<<std::endl;
        return false;
    }
}


void removeFifo(const std::string& path)
{
    int n=unlink(path.c_str());
    assert(n==0);
    (void)n;
}
