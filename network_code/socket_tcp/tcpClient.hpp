#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

enum
{
    SOCKET_ERR,
    CONNECT_ERR,
    USAGE_ERR
};

class TcpClient
{
public:
    TcpClient(const string &ip, const uint16_t& port) : _port(port), _ip(ip), _sockfd(-1) {}

    void initClient()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd == -1)
        {
            cerr << "socket err" << endl;
            exit(SOCKET_ERR);
        }
        cout << "socket succes ->" << _sockfd << endl;
    }

    void start()
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(_ip.c_str());
        server.sin_port = htons(_port);
        socklen_t len = sizeof server;
        if (connect(_sockfd, (struct sockaddr *)&server, len) != 0)
        {
            cerr << "connect err" << endl;
        }
        else
        {
            string buff;
            while (true)
            {
                cout<<"Enter# ";
                getline(cin, buff);
                write(_sockfd, buff.c_str(), buff.size());

                char buffer[1024];
                ssize_t n = read(_sockfd, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    cout << "Server 回显# " << buffer << endl;
                }
                else
                {
                    break;
                }
            }
        }
    }

private:
    uint16_t _port;
    string _ip;
    int _sockfd;
};

