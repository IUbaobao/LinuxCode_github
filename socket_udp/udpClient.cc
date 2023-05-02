#include "udpClient.hpp"

#include <memory>

using namespace Client;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\n";
}

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        exit(3);
    }

    string serverip=argv[1];
    uint16_t serverport=atoi(argv[2]);

    unique_ptr<udpClient> ucli(new udpClient(serverport,serverip));
    ucli->initClient();
    ucli->run();

    return 0;
}

