#include "udpServer.hpp"
#include <memory>
#include <unordered_map>
#include <fstream>
#include <signal.h>
#include <cstdio>
#include "onlineUser.hpp"
using namespace std;
using namespace Server;

// const string dictTxt = "./dict.txt";
// unordered_map<string, string> dict;
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << "local_port\n\n"
         << endl;
}

// bool cutString(const string line, string *s1, string *s2, const string &seq)
// {
//     auto it = line.find(seq);
//     if (it == string::npos)
//         return false;
//     *s1 = line.substr(0, it);
//     *s2 = line.substr(it + seq.size());
//     return true;
// }

// void InitDict()
// {
//     ifstream in(dictTxt, ios::binary);
//     if (!in.is_open())
//     {
//         cout << "oppen file" << dictTxt << "error" << endl;
//         exit(OPEN_ERR);
//     }
//     string line;
//     string key, val;
//     while (getline(in, line))
//     {
//         if (cutString(line, &key, &val, ":"))
//         {
//             dict.insert(make_pair(key, val));
//         }
//     }
//     in.close();
//     cout << "load dict success" << endl;
// }

// static void debugPrint()
// {
//     for (auto &e : dict)
//     {
//         cout << e.first << "->" << e.second << endl;
//     }
// }

// void reload(int signo) // 热加载
// {
//     (void)signo;
//     InitDict();
// }

// // demo1： 英译汉
// void handlerMessage(int sockfd, string clientip, uint16_t clienport, string message)
// {
//     string response_message;
//     auto it = dict.find(message);
//     if (it != dict.end())
//         response_message = it->second;
//     else
//         response_message = "unknown";
//     // 返回信息
//     struct sockaddr_in client;
//     memset(&client,0,sizeof client);
//     client.sin_family = AF_INET;
//     client.sin_addr.s_addr = inet_addr(clientip.c_str());
//     client.sin_port = htons(clienport);
//     socklen_t len = sizeof client;
//     sendto(sockfd, response_message.c_str(), response_message.size(), 0, (sockaddr *)&client, len);
// }

// demo2: 命令行解析
// void execCommand(int sockfd, string clientip, uint16_t clienport, string cmd)
// {
//     string response;
//     FILE* fp=popen(cmd.c_str(),"r");
//     if(fp==nullptr) response=cmd+"exec failed";
//     char line[1024];
//     while(fgets(line,sizeof(line),fp))
//     {
//         response+=line;
//     }
//     pclose(fp);
//     // 返回信息
//     struct sockaddr_in client;
//     memset(&client,0,sizeof client);
//     client.sin_family = AF_INET;
//     client.sin_addr.s_addr = inet_addr(clientip.c_str());
//     client.sin_port = htons(clienport);
//     socklen_t len = sizeof client;
//     sendto(sockfd, response.c_str(), response.size(), 0, (sockaddr *)&client, len);

// }

onlineUser users;

void routeMessage(int sockfd, string clientip, uint16_t clienport, string message)
{
    if(message=="online")//上线
        users.add(clientip,clienport);
    if(message=="offline")//下线
        users.del(clientip,clienport);
    if(users.isOnline(clientip,clienport))
    {
        users.broadcastMessage(sockfd,clientip,clienport,message);
    }
    else
    {
        string response="你还没上线,请先上线,运行:online\n";
        struct sockaddr_in client;
        memset(&client,0,sizeof client);
        client.sin_family = AF_INET;
        client.sin_addr.s_addr = inet_addr(clientip.c_str());
        client.sin_port = htons(clienport);
        socklen_t len = sizeof client;
        sendto(sockfd, response.c_str(), response.size(), 0, (sockaddr *)&client, len);
    }
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    // signal(2, reload); // ctrl + c 重新加载词库
    uint16_t port = atoi(argv[1]);
    // string ip=argv[2];
    // InitDict();
    // debugPrint();
    unique_ptr<udpServer> usvr(new udpServer(port, routeMessage));
    usvr->initServer();
    usvr->start();
    return 0;
}
