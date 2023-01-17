#include "comm.hpp"

using namespace std;

int main()
{
    int fd=open(NAMED_PIPE,O_RDONLY);
    assert(fd>=0);
    char buff[1024];

    while(1)
    {
        ssize_t n=read(fd,buff,sizeof(buff)-1);
        if(n==0)
        {
            break;
        }
        buff[n]=0;
        cout<<buff<<endl;
    }
    close(fd);
    return 0;
}