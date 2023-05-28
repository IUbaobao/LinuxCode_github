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
#include "protocol.hpp"
using namespace std;

enum
{
    SOCKET_ERR,
    CONNECT_ERR,
    USAGE_ERR
};

class calClient
{
public:
    calClient(const string &ip, const uint16_t& port) : _port(port), _ip(ip), _sockfd(-1) {}

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
            string line;
            string inbuffer;
            while (true)
            {
                cout<<"mycal# ";
                getline(cin, line);

                Request req=ParseLine(line);

                std::string content;
                req.serialize(&content);
                std::string send_string =enLength(content);
                send(_sockfd,send_string.c_str(),send_string.size(),0);
                // write(_sockfd, buff.c_str(), buff.size());

                std::string package, text;
                if (!recvPackage(_sockfd, inbuffer, &package))
                    continue;
                if (!deLength(package, &text))
                    continue;
                // "exitcode result"
                Response resp;
                resp.deserialize(text);
                std::cout << "exitCode: " << resp._exitcode << std::endl;
                std::cout << "result: " << resp._result << std::endl;
            }
        }
    }


    Request ParseLine(const std::string&line)
    {
        // 小版本的状态机！
        //"1+1" "123*456" "12/0"
        int status=0;// 0:操作符之前，1:碰到了操作符 2:操作符之后
        std::string left,right;
        char op;
        int i=0;
        while(i<line.size())
        {
            switch(status)
            {
                case 0:
                {
                    if(!isdigit(line[i]))
                    {
                        op=line[i];
                        status=1;
                    }
                    else
                    left+=line[i++];
                }
                break;
                case 1:
                    i++;
                    status=2;
                    break;
                case 2:
                    right+=line[i++];
                    break;
            }
        }
        std::cout << std::stoi(left)<<" " << std::stoi(right) << " " << op << std::endl;
        return Request(std::stoi(left), std::stoi(right), op);
    }

private:
    uint16_t _port;
    string _ip;
    int _sockfd;
};

