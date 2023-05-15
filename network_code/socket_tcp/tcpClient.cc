#include "tcpClient.hpp"
#include <memory>
using namespace std;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " serverip serverport\n\n";
}
// ./tcpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);

    unique_ptr<TcpClient> tcli(new TcpClient(serverip, serverport));
    tcli->initClient();
    tcli->start();
    return 0;
}

