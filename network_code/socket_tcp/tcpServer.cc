#include "tcpServer.hpp"
#include <memory>



static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

int main(int argc,char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    std::unique_ptr<tcpServer> tser(new tcpServer(port));
    tser->initServer();
    tser->start();
    return 0;
}
