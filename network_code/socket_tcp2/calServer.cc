#include "calServer.hpp"
#include <memory>
#include <map>

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

bool cal(const Request &req, Response &resp)
{
    resp._exitcode=OK;
    resp._result=OK;
    std::map<char,std::function<int(int, int)>> opFunMap =
        {
            {'+', [](int x, int y){ return x + y; }},
            {'-', [](int x, int y){ return x - y; }},
            {'*', [](int x, int y){ return x * y; }},
            {'/', [&](int x, int y)
            {if(y==0) {resp._exitcode=DIV_ZERO; return y;} return x / y; }},
            {'%', [&](int x, int y)
            { if(y==0) {resp._exitcode=MOD_ZERO; return y;} return x % y; }}
        };
 
    if(opFunMap.find(req._op)!=opFunMap.end())
    {
        resp._result=opFunMap[req._op](req._x,req._y);
    }
    else
    {
        resp._exitcode=OP_ERROR;
    }

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

    std::unique_ptr<calServer> tser(new calServer(port));
    tser->initServer();
    tser->start(cal);
    return 0;
}
