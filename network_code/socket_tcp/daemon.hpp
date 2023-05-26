#pragma once 

#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//相当于一个垃圾桶
#define DEV "/dev/null"

//守护进程
void daemonSelf(const char*currPath=nullptr)
{
    //1. 让调用进程忽略异常的信号
    signal(SIGPIPE,SIG_IGN);

    //2.让自己不是组长，setsid
    if(fork()>0) exit(0);

    //子进程---守护进程，精灵进程，本质就是孤儿进程的一种
    pid_t n=setsid();
    assert(n!=-1);

    //3.守护进程是脱离终端的，关闭或者重定向以前默认打开的文件
    int fd=open(DEV,O_RDWR);
    if(fd>0)
    {
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        close(fd);
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }

    if(currPath) chdir(currPath);
}