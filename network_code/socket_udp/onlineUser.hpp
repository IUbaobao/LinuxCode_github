#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;
class user
{
public:
    user(const string &ip, const uint16_t &port) : _ip(ip), _port(port) {}
    string getip(){return _ip;}
    uint16_t getport(){return _port;}
private:
    string _ip;
    uint16_t _port;
};

class onlineUser
{
public:
    void add(const string& ip,const uint16_t& port)
    {
        string key=ip+"-"+to_string(port);
        _users.insert(make_pair(key,user(ip,port)));
    }

    void del(const string& ip,const uint16_t& port)
    {
        string key=ip+"-"+to_string(port);
        _users.erase(key);
    }
    bool isOnline(const string& ip,const uint16_t& port)
    {
        string key=ip+"-"+to_string(port);

        return _users.find(key)!=_users.end();
    }
    
    void broadcastMessage(int sockfd,const string& ip,const uint16_t& port,const string& message)
    {
        for(auto& it:_users)
        {
            struct sockaddr_in client;
            bzero(&client,sizeof client);
            string response=ip+"["+to_string(port)+"]# "+message;

            client.sin_addr.s_addr=inet_addr(it.second.getip().c_str());
            client.sin_family=AF_INET;
            client.sin_port=htons(it.second.getport());
            socklen_t len=sizeof client;
            sendto(sockfd,response.c_str(),response.size(),0,(sockaddr*)&client,len);
        }
    }
private:
    unordered_map<string,user> _users;
};