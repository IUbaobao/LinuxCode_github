#include  <iostream>
#include "BlockQueue.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <ctime>

using namespace std;


void* producer(void* args)
{
    //生产者
    BlockQueue<int>* bq=static_cast<BlockQueue<int>*>(args);
    while(true)
    {
        int x=rand()%10+1;
        bq->push(x);
        cout<<"生产者：生产了一个："<<x<<endl;
        // sleep(1);
    }
    return nullptr;
}

void* consumer(void* args)
{
    //消费者
    BlockQueue<int>* bq=static_cast<BlockQueue<int>*>(args);
    while(true)
    {
        int x;
        bq->pop(&x);
        cout<<"消费者：消费了一个:"<<x<<endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    srand((unsigned long)time(nullptr)^getpid());
    BlockQueue<int>* bq=new BlockQueue<int>(5);
    pthread_t pd,cs;
    pthread_create(&pd,nullptr,producer,bq);
    pthread_create(&cs,nullptr,consumer,bq);

    pthread_join(pd,nullptr);
    pthread_join(cs,nullptr);
    delete bq;
    return 0;
}


