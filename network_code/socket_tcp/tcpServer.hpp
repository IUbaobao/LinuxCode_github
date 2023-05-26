#pragma once 

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>
#include "log.hpp"
#include "ThreadPool.hpp"
#include "Task.hpp"
using namespace std;

enum{SOCK_ERR,BIND_ERR,LISTEN_ERR,USAGE_ERR};


class tcpServer;

class threadData
{
public:
    threadData( tcpServer* self,int sock):_self(self),_sock(sock){}
public:
    tcpServer* _self;
    int _sock;
};

static const uint16_t gport=8080;
static const int gbacklog=5;

class tcpServer
{
public:
    tcpServer(const uint16_t& port=gport) :_port(port),_listsocket(-1){}

    void initServer()
    {
        // 1. 创建socket文件套接字对象
        _listsocket=socket(AF_INET,SOCK_STREAM,0);
        if(_listsocket==-1)
        {
            // cerr<<"socket err"<<endl;
            logMessage(FATAL,"create socket error");
            exit(SOCK_ERR);
        }
        // cout<<"socket success ->"<<_listsocket<<endl;
        logMessage(NORMAL,"create socket success: %d",_listsocket);

        // 2. bind绑定自己的网络信息
        struct sockaddr_in local;
        memset(&local,0,sizeof local);
        local.sin_family=AF_INET;
        local.sin_port=htons(_port);
        local.sin_addr.s_addr=INADDR_ANY;
        socklen_t len=sizeof local;
        int n=bind(_listsocket,(struct sockaddr*)&local,len);
        if(n==-1)
        {
            // cerr<<"bind err"<<endl;
            logMessage(FATAL,"bind socket error");
            exit(BIND_ERR);
        }
        // cout<<"bind success"<<endl;
        logMessage(NORMAL,"bind socket success");

        // 3. 设置socket 为监听状态
        if(listen(_listsocket,gbacklog)<0)
        {
            // cerr<<"listen err"<<endl;
            logMessage(FATAL,"listen socket error");
            exit(LISTEN_ERR);
        }
        // cout<<"listen success"<<endl;
        logMessage(NORMAL,"listen success");

    }

    void start()
    {
        //线程池版
        ThreadPool<Task>::getInstance()->run();
        // signal(SIGCHLD,SIG_IGN);
        while(true)
        {
             // 4. server 获取新链接
            // sock, 和client进行通信的fd
            struct sockaddr_in peer;
            memset(&peer,0,sizeof peer);
            socklen_t len=sizeof peer;
            int sock=accept(_listsocket,(sockaddr*)&peer,&len);
            if(sock==-1)
            {
                // cerr<<"accept err"<<endl;
                logMessage(FATAL,"accept socket error");
                continue;
            }
            // cout<<"accept succes ->"<<sock<<endl;
            logMessage(NORMAL,"accept socket success: %d",sock);
            //5. 这里就是一个sock，未来通信我们就用这个sock，面向字节流的，后续全部都是文件操作！
            //
            //多进程版
            //方式1：孙子进程执行
            // pid_t pid=fork();
            // if(pid==0)
            // {
            //     if(fork()>0)exit(1);
            //     serverIO(sock);
            // }

            // pid=waitpid(pid,nullptr,0);
            // if(pid>0)
            // {
            //     cout<<"waitpid sucess"<<endl;
            // }
            
            //方式2：忽略子进程信号
            // if(pid==0)
            // {
            //     serverIO(sock);
            // }

            //多线程版
            // pthread_t tid;
            // threadData* td=new threadData(this,sock);
            // int n=pthread_create(&tid,nullptr,threadRoutine,td);
            // if(n==0)
            // {
            //     // cout<<"pthread_create success "<<endl;
            //     logMessage(NORMAL,"pthread_create success");
            // }

            ThreadPool<Task>::getInstance()->push(Task(sock,serverIO));

        }
    }

    // static void* threadRoutine(void* args)
    // {
    //     threadData* td=static_cast<threadData*>(args);
    //     td->_self->serverIO(td->_sock);
    //     delete td;
    //     return nullptr;
    // }

private:
    uint16_t _port;
    int _listsocket;
};