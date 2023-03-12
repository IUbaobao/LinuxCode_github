#include "comm.hpp"

int main()
{
    //生成唯一性的key值
    key_t key=getkey();
    //创建共享内容
    int shmid=createshm(key);
    //挂接
    char* start=(char*)attachShm(shmid);
    //接收内容
    int cnt=0;
    while(1)
    {
        printf("%s\n",start);
        cnt++;
        sleep(1);
        if(cnt==12)
        {
            break;
        }
    }
    // 去关联
    deltachShm(start);
    sleep(5);
    // 删除共享内存
    delShm(shmid);

    return 0;
}