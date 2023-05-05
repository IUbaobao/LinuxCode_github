#include "udpServer.hpp"
#include <memory>
#include <unordered_map>
#include <fstream>
using namespace std;
using namespace Server;

const string dictTxt="./dict.txt";
unordered_map<string,string> dict;
static void Usage(string proc)
{
    cout<<"\nUsage:\n\t"<<proc<<"local_port\n\n"<<endl;
}


bool cutString(const string line,string* s1,string*s2,const string& seq)
{
    auto it=line.find(seq);
    if(it==string::npos) 
    return false;
    *s1=line.substr(0,it);
    *s2=line.substr(it+seq.size());
    return true;
}

void InitDict()
{
    ifstream in(dictTxt,ios::binary);
    if(!in.is_open())
    {
        cout<<"oppen file"<<dictTxt<<"error"<<endl;
        exit(OPEN_ERR);
    }
    string line;    
    string key,val;
    while(getline(in,line))
    {
        if(cutString(line,&key,&val,":"))
        {
            dict.insert(make_pair(key,val));
        }
    }
    in.close();
    cout<<"load dict success"<<endl;
}


static void debugPrint()
{
    for(auto& e:dict)
    {
        cout<<e.first<<"->"<<e.second<<endl;
    }
} 

void handlerMessage(int sockfd,string clientip,uint16_t clienport,string message)
{
    string response_message;
    auto it=dict.find(message);
    if(it!=dict.end())
    {
        response_message=it->second;
    }
    else
    {
        response_message="unknown";
    }

    //返回信息
    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_addr.s_addr=inet_addr(clientip.c_str());
    client.sin_port=htons(clienport);
    socklen_t len=sizeof client;
    sendto(sockfd,response_message.c_str(),response_message.size(),0,(sockaddr*)&client,len);
}

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port=atoi(argv[1]);
    // string ip=argv[2];
    InitDict();
    debugPrint();
    unique_ptr<udpServer> usvr(new udpServer(port,handlerMessage));
    usvr->initServer();
    usvr->start();
    return 0;
}

