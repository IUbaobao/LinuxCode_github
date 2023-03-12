#include "comm.hpp"

int main()
{
    //生成key
    key_t key=getkey();
    //生成shmid
    int shmid=getshm(key);
    //挂接
    char* start=(char*)attachShm(shmid);
    int cnt=0;
    //写内容
    while(1)
    {
        sprintf(start,"我是client,我的pid是%d,我正在向你发送内容%d",getpid(),++cnt);
        sleep(1);
        if(cnt==10)
        {
            break;
        }
    }

    //断开连接
    deltachShm(start);
    return 0;
}
