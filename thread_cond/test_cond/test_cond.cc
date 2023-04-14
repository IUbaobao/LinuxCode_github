#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;


int ticket=1000;

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;//条件变量---全局初始化方式

void* start_routine(void* args)
{
    char* s=static_cast<char*>(args);
    while(true)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond,&lock);//在条件变量下等待，为什么要传锁，主要是用于解锁..后续慢慢体会
        cout<<s<<":正在抢票....."<<"ticket:"<<ticket<<endl;
        //判断暂时忽略
        --ticket;
        //sleep(1);
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    // pthread_t t1,t2;
    // pthread_create(&t1,nullptr,start_routine,(void*)"thread 1");
    // pthread_create(&t2,nullptr,start_routine,(void*)"thread 2");
#define NUM 5
    vector<pthread_t> tds(NUM);
    for(int i=0;i<NUM;++i)
    {
        char* buff=new char[64];
        sprintf(buff,"thread %d",i+1);
        pthread_create(&tds[i],nullptr,start_routine,(void*)buff);
    }
    sleep(1);
    while(true)
    {
        pthread_cond_signal(&cond);//主线程在这条件变量下每隔1s唤醒一个
        cout<<"main thread wakeup one thread..."<<endl;
        sleep(1);
    }

    for(const auto& tid:tds)
    {
        pthread_join(tid,nullptr);
    }

    return 0;
}
