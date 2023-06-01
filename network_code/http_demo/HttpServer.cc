#include "HttpServer.hpp"
#include <memory>
#include <map>

using namespace std;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

bool Get(const HttpRequest &req, HttpResponse &resp)
{
    cout<<"---------------------------http start-----------------------------"<<endl;
    std::cout<<req.inbuffer<<std::endl;
    cout<<"---------------------------http end---------------------------------"<<endl;

    string respline ="http/1.1 200 OK\r\n";
    string respheader="Content-Type: text/html\r\n";
    string respblank ="\r\n";
    string body="<html lang=\"en\"><head><meta charset=\"UTF-8\"><title>for test</title><h1>hello world</h1></head><body><p>!!!!!http test!!!!!</p></body></html>";

    resp.outbuffer+=respline;
    resp.outbuffer+=respheader;
    resp.outbuffer+=respblank;
    resp.outbuffer+=body;
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    std::unique_ptr<httpServer> tser(new httpServer(Get,port));
    tser->initServer();
    tser->start();
    return 0;
}
