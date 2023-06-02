#include "HttpServer.hpp"
#include <memory>
#include <map>

using namespace std;

const string html_404="./wwwroot/404.html";

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}


std::string suffixToDesc(const std::string& suffix)
{   
    std::string ct="Content-Type: ";
    if(suffix==".html")
        ct+="text/html";
    else if(suffix==".png")
        ct+="image/png";
    
    ct+="\r\n";
    return ct;
}


// 1. 服务器和网页分离，html
// 2. url -> / : web根目录
// 3. 我们要正确的给客户端返回资源类型，我们首先要自己知道！所有的资源都有后缀！！

bool Get(const HttpRequest &req, HttpResponse &resp)
{
    cout<<"---------------------------http start-----------------------------"<<endl;
    std::cout<<req.inbuffer;
    std::cout<<"method:"<<req.method<<endl;
    std::cout<<"url:"<<req.url<<endl;
    std::cout<<"httpversion:"<<req.httpversion<<endl;
    std::cout<<"path:"<<req.path<<endl;
    std::cout<<"suffix:"<<req.suffix<<endl;
    std::cout<<"size:"<<req.size<<"字节"<<endl;
    cout<<"---------------------------http end---------------------------------"<<endl;

    string respline ="http/1.1 200 OK\r\n";
    // string respheader="Content-Type: text/html\r\n";
    string respheader=suffixToDesc(req.suffix);
    string respblank ="\r\n";
    // string body="<html lang=\"en\"><head><meta charset=\"UTF-8\"><title>for test</title><h1>hello world</h1></head><body><p>!!!!!http test!!!!!</p></body></html>";
    
    string body;
    body.resize(req.size+1);
    if(!Util::readFile(req.path,(char*)body.c_str(),req.size))
    {
        int size=1024;
        body.resize(size);
        Util::readFile(html_404,(char*)body.c_str(),size);
    }
    respheader+="Content-Length: ";
    respheader+=std::to_string(req.size);
    respheader+="\r\n";
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
