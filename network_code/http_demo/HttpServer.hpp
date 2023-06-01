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

typedef std::function<bool(const HttpRequest, HttpResponse &resp)> func_t;

class httpServer
{
public:
    httpServer(func_t func, const uint16_t &port = gport) : _port(port), _listsock(-1), _func(func) {}

    void initServer()
    {
        // 1. 创建socket文件套接字对象
        _listsock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listsock == -1)
        {
            exit(SOCK_ERR);
        }

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
            exit(BIND_ERR);
        }

        // 3. 设置socket 为监听状态
        if (listen(_listsock, gbacklog) < 0)
        {
            exit(LISTEN_ERR);
        }
    }

    void HandlerHttp(int sock)
    {
        char buffer[4096];
        HttpRequest req;
        HttpResponse resp;
        size_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            req.inbuffer = buffer;
            _func(req, resp);
            send(sock,resp.outbuffer.c_str(),resp.outbuffer.size(),0);
        }
    }

    void start()
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
                continue;
            }

            // 5. 这里就是一个sock，未来通信我们就用这个sock，面向字节流的，后续全部都是文件操作！
            //
            // 多进程版
            pid_t pid = fork();
            if (pid == 0)
            {
                close(_listsock);
                if (fork() > 0)
                    exit(1);

                HandlerHttp(sock);
                close(sock);
                exit(0);
            }

            close(sock);
            pid = waitpid(pid, nullptr, 0);
        }
    }

private:
    uint16_t _port;
    int _listsock;
    func_t _func;
};