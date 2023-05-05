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

using namespace std;

namespace Client
{
    class udpClient
    {
    public:
        udpClient(const uint16_t& server_port,const string& server_ip)
        :_serverip(server_ip),_serverport(server_port),_sockfd(-1),_quit(false)
        {}

        void initClient()
        {
            //创建socket
            _sockfd=socket(AF_INET,SOCK_DGRAM,0);
            if(_sockfd==-1)
            {
                cerr<<"socket err:"<<errno<<strerror(errno)<<endl;
                exit(2);
            }
            cout<<"socket success:"<<":"<<_sockfd<<endl;

        }
        void run()
        {
            struct  sockaddr_in server;
            memset(&server,0,sizeof(server));
            server.sin_family=AF_INET;
            server.sin_port=htons(_serverport);
            server.sin_addr.s_addr=inet_addr(_serverip.c_str());

            string message;
            while(!_quit)
            {
                cout<<"Please Enter# ";
                cin>>message;
                sendto(_sockfd,message.c_str(),message.size(),0,(struct sockaddr*)&server,sizeof(server));
                char buff[1024];
                socklen_t len=sizeof server;
                int s=recvfrom(_sockfd,buff,sizeof(buff)-1,0,(struct sockaddr*)&server,&len);
                if(s>=0)
                {
                    buff[s]=0;
                    cout<<_serverip<<"["<<_serverport<<"]#"<<buff<<endl;
                }
            }


        }
    private:
        int _sockfd;
        string _serverip;
        uint16_t _serverport;
        bool _quit;
    };
}




