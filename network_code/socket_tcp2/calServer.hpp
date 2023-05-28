#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "log.hpp"
#include "protocol.hpp"
using namespace std;

enum
{
    SOCK_ERR,
    BIND_ERR,
    LISTEN_ERR,
    USAGE_ERR
};

static const uint16_t gport = 8080;
static const int gbacklog = 5;

typedef std::function<bool(const Request, Response &resp)> func_t;

void handlerEntery(int sock, func_t func)
{
    std::string inbuffer;
    while (true)
    {
        // 1. 读取："content_len"\r\n"x op y"\r\n
        // 1.1 你怎么保证你读到的消息是 【一个】完整的请求
        std::string req_text, req_str;
        
         // 1.2 我们保证，我们req_text里面一定是一个完整的请求："content_len"\r\n"x op y"\r\n
        if (!recvPackage(sock, inbuffer, &req_text))
            return;
        std::cout << "带报头的请求：\n" << req_text << std::endl;
        if (!deLength(req_text, &req_str))
            return;
        std::cout << "去掉报头的正文：\n" << req_str << std::endl;
        
        // 2. 对请求Request，反序列化
        // 2.1 得到一个结构化的请求对象
        Request req;
        if (!req.deserialize(req_str))
            return;

        // 3. 计算机处理，req.x, req.op, req.y --- 业务逻辑
        // 3.1 得到一个结构化的响应
        Response resp;
        func(req, resp); // req的处理结果，全部放入到了resp
        // 4.对响应Response，进行序列化
        // 4.1 得到了一个"字符串"
        std::string resp_str;
        resp.serialize(&resp_str);
        std::cout << "计算完成, 序列化响应: " <<  resp_str << std::endl;
        // 5. 然后我们在发送响应
        // 5.1 构建成为一个完整的报文
        std::string send_string=enLength(resp_str);
        std::cout << "构建完成完整的响应\n" <<  send_string << std::endl;
        send(sock,send_string.c_str(),send_string.size(),0);
    }
}

class calServer
{
public:
    calServer(const uint16_t &port = gport) : _port(port), _listsock(-1) {}

    void initServer()
    {
        // 1. 创建socket文件套接字对象
        _listsock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listsock == -1)
        {
            // cerr<<"socket err"<<endl;
            logMessage(FATAL, "create socket error");
            exit(SOCK_ERR);
        }
        // cout<<"socket success ->"<<_listsock<<endl;
        logMessage(NORMAL, "create socket success: %d", _listsock);

        // 2. bind绑定自己的网络信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        socklen_t len = sizeof local;
        int n = bind(_listsock, (struct sockaddr *)&local, len);
        if (n == -1)
        {
            // cerr<<"bind err"<<endl;
            logMessage(FATAL, "bind socket error");
            exit(BIND_ERR);
        }
        // cout<<"bind success"<<endl;
        logMessage(NORMAL, "bind socket success");

        // 3. 设置socket 为监听状态
        if (listen(_listsock, gbacklog) < 0)
        {
            // cerr<<"listen err"<<endl;
            logMessage(FATAL, "listen socket error");
            exit(LISTEN_ERR);
        }
        // cout<<"listen success"<<endl;
        logMessage(NORMAL, "listen success");
    }

    void start(func_t func)
    {
        while (true)
        {
            // 4. server 获取新链接
            // sock, 和client进行通信的fd
            struct sockaddr_in peer;
            memset(&peer, 0, sizeof peer);
            socklen_t len = sizeof peer;
            int sock = accept(_listsock, (struct sockaddr *)&peer, &len);
            if (sock == -1)
            {
                // cerr<<"accept err"<<endl;
                logMessage(FATAL, "accept socket error");
                continue;
            }
            // cout<<"accept succes ->"<<sock<<endl;
            logMessage(NORMAL, "accept socket success: %d", sock);
            // 5. 这里就是一个sock，未来通信我们就用这个sock，面向字节流的，后续全部都是文件操作！
            //
            // 多进程版
            pid_t pid = fork();
            if (pid == 0)
            {
                close(_listsock);
                if (fork() > 0)
                    exit(1);

                handlerEntery(sock,func);
                close(sock);
                exit(0);
            }

            close(sock);
            pid = waitpid(pid, nullptr, 0);
            if (pid > 0)
            {
                cout << "waitpid sucess" << endl;
            }
        }
    }

private:
    uint16_t _port;
    int _listsock;
};