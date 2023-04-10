#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include <assert.h>
using namespace std;

void* run(void* agrc)
{
    //int cnt=8;
    while(1)
    {
        //线程的pid和主线程的pid是相等的---从宏观上理解它们属于同一个进程，不同的执行流
        //从底层上看：Linux上并没有真正意义上的线程，而是用进程PCB来模拟线程的，站在cpu角度上看每一个PCB都可以称之为轻量级进程
        //cpu调度的时候，是以那一个id作为标识符表示一个特定的执行流？---LWP (Light weight process)--轻量级线程  ps -aL 即可查看线程LWP 
        cout<<"I am thread....."<<(char*)agrc<<"我的pid:"<<getpid()<<endl;
        printf("pthread_self:%x\n",pthread_self());
        sleep(1);
    }
    return nullptr;
}
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *
// (*start_routine)(void*), void *arg);
// 参数
// thread:返回线程ID
// attr:设置线程的属性，attr为NULL表示使用默认属性
// start_routine:是个函数地址，线程启动后要执行的函数
// arg:传给线程启动函数的参数
// 返回值：成功返回0；失败返回错误码

int main()
{
    pthread_t tid;
    int ret=pthread_create(&tid,nullptr,run,(void*)"线程1");
    assert(ret==0);
    (void)ret;
    while(1)
    {
        cout<<"I am main thread........我的pid:"<<getpid()<<endl;
        printf("线程tid:0x%x\n",tid);//这个tid是一个地址..后续看..(此tid并非LWP)
        sleep(1);
    }
    return 0;
}
