#pragma once 

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

enum{SOCK_ERR,BIND_ERR,LISTEN_ERR,USAGE_ERR};

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
            cerr<<"socket err"<<endl;
            exit(SOCK_ERR);
        }
        cout<<"socket succes ->"<<_listsocket<<endl;

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
            cerr<<"bind err"<<endl;
            exit(BIND_ERR);
        }
        cout<<"bind succes"<<endl;
        // 3. 设置socket 为监听状态
        if(listen(_listsocket,gbacklog)<0)
        {
            cerr<<"listen err"<<endl;
            exit(LISTEN_ERR);
        }
        cout<<"listen succes"<<endl;
    }

    void start()
    {
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
                cerr<<"accept err"<<endl;
                continue;
            }
            cout<<"accept succes ->"<<sock<<endl;
            //5. 这里就是一个sock，未来通信我们就用这个sock，面向字节流的，后续全部都是文件操作！
            serverIO(sock);
            
        }
    }


    void serverIO(int sock)
    {
        char buff[1024];
        while(true)
        {
            ssize_t n=read(sock,buff,sizeof(buff)-1);
            if(n>0)
            {
                //目前先简单把数据转成字符串输入
                buff[n]=0;
                cout<<"recv message:"<<buff<<endl;

                string outbuff=buff;
                write(sock,outbuff.c_str(),outbuff.size());
            }
            else if(n ==0)
            {
                cout<<"client quit, me too!"<<endl;
                break;
            }
        }
        close(sock);//对一个已经使用完毕的sock，我们要关闭这个sock，要不然会导致，文件描述符泄漏
    }
private:
    uint16_t _port;
    int _listsocket;
};