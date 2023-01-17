#include "comm.hpp"

using namespace std;

//负责写内容到管道
int main()
{
    //创建管道文件
    bool r=createFifo(NAMED_PIPE);
    assert(r);

    int fd=open(NAMED_PIPE,O_WRONLY|O_TRUNC);
    assert(fd>=0);

    string buff;

    while(true)
    {
        cin>>buff;
        write(fd,buff.c_str(),buff.size());
    }
    close(fd);
    return 0;
}
