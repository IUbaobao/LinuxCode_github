#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <cstdio>
#include "log.hpp"

void serverIO(int sock)
{
    char buff[1024];
    while (true)
    {
        ssize_t n = read(sock, buff, sizeof(buff) - 1);
        if (n > 0)
        {
            // 目前先简单把数据转成字符串输入
            buff[n] = 0;
            std::cout << "recv message:" << buff << std::endl;

            std::string outbuff = buff;
            write(sock, outbuff.c_str(), outbuff.size());
        }
        else if (n == 0)
        {
            // cout<<"client quit, me too!"<<endl;
            logMessage(NORMAL, "client quit sock:%d closed", sock);
            break;
        }
    }
    close(sock); // 对一个已经使用完毕的sock，我们要关闭这个sock，要不然会导致，文件描述符泄漏
}


class Task
{
public:
    using func_t = std::function<void(int)>;
    Task()
    {
    }
    Task(int sock, func_t func)
        : _sock(sock), _callback(func)
    {
    }
    void operator()()
    {
        _callback(_sock);
    }
private:
    int _sock;
    func_t _callback;
};