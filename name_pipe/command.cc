#include "comm.hpp"


//argc==1 没有额外参数的时候创建管道
//argc>1 有参数的时候删除管道
int main(int argc,const char* argv[])
{   
    if(argc==1)
    {
        bool flag=createFifo(NAMED_PIPE);
        assert(flag==true);
        exit(0);
    }

    int n =unlink(NAMED_PIPE);
    assert(n==0);
    (void)n;

    return 0;
}