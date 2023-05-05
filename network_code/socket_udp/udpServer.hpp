#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
using namespace std;

enum
{
    USAGE_ERR,
    SOCK_ERR,
    BIND_ERR,
    OPEN_ERR,
};

namespace Server
{
    typedef function<void (int,string,uint16_t,string)> func_t;
    static const string defaultIp = "0.0.0.0";
    static const int num = 1024;
    class udpServer
    {
    public:
        udpServer(uint16_t port, func_t callback,string ip = defaultIp)
            : _port(port), _ip(ip), _sockfd(-1),_callback(callback)
        {}

        void initServer()
        {
            // 1.创建 socket 文件描述符 (TCP/UDP, 客户端 + 服务器)
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sockfd == -1)
            {
                cerr << "socket err" << errno << strerror(errno) << endl;
                exit(SOCK_ERR);
            }
            cout << "socket sucess:" << _sockfd << endl;

            // 2. 绑定port---未来的服务器要明确port，不能随意更改
            struct sockaddr_in local;
            bzero(&local, sizeof(local));
            local.sin_port = htons(_port);                         // 端口号
            local.sin_family = AF_INET;                     //
            // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // ip地址，这里是string，但是网络传输中用的是4字节的整数，并且是要大端字序
                                                            // string->uint32_t->大端字序---用inet_addr()一步到位
            local.sin_addr.s_addr = htonl(INADDR_ANY);                                                
            int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
            if (n == -1)
            {
                cerr << "bind err " << errno << strerror(errno) << endl;
                exit(BIND_ERR);
            }
        }
        void start()
        {
            char buff[num];
            while (true)
            {
                // 读取数据
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                ssize_t s = recvfrom(_sockfd, buff, sizeof(buff) - 1, 0, (struct sockaddr *)&peer, &len);
                if (s > 0)
                {
                    buff[s] = 0;
                    string clientip = inet_ntoa(peer.sin_addr); // 网络字序--》点分十进制ip
                    uint16_t clientport = ntohs(peer.sin_port);
                    string message = buff;

                    cout << clientip << "[" << clientport << "]#" << message << endl;
                    _callback(_sockfd,clientip,clientport,message);
                }
            }
        }

    private:
        uint16_t _port;
        string _ip;
        int _sockfd;
        func_t _callback;//回调
    };
}



