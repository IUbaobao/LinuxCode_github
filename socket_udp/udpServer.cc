#include "udpServer.hpp"
#include <memory>
using namespace std;
using namespace Server;


static void Usage(string proc)
{
    cout<<"\nUsage:\n\t"<<proc<<"local_port\n\n"<<endl;
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
    unique_ptr<udpServer> usvr(new udpServer(port));
    usvr->initServer();
    usvr->start();
    return 0;
}

