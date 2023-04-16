#include <iostream>
#include <pthread.h>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include "RingQueue.hpp"
#include "Task.hpp"
using namespace std;

void *producer(void *args)
{
    // 生产者
    //  RingQueue<int>*rq=static_cast<RingQueue<int>*>(args);
    RingQueue<CallTask> *rq = static_cast<RingQueue<CallTask> *>(args);

    while (true)
    {
        // sleep(2);
        int x = rand() % 10;
        int y = rand() % 5;
        char op = oper[rand() % oper.size()];
        CallTask t(x, y, op, mymath);
        rq->push(t);
        cout << "生产者:生产了一个数据：" << t.toSting() << endl;
        sleep(1);
    }
    return nullptr;
}

void *consumer(void *args)
{
    // 消费者
    //  RingQueue<int>*rq=static_cast<RingQueue<int>*>(args);
    RingQueue<CallTask> *rq = static_cast<RingQueue<CallTask> *>(args);
    while (true)
    {
        // sleep(3);
        CallTask t;
        rq->pop(&t);
        cout << "消费者:消费了一个数据: " << t() << endl;
    }
    return 0;
}

int main()
{
    srand((unsigned int)time(nullptr) ^ 0x123 ^ getpid());
    // RingQueue<int>* rq=new RingQueue<int>(5);
    RingQueue<CallTask> *rq = new RingQueue<CallTask>(3);

    pthread_t c[5], s[5];
    for (int i = 0; i < 5; ++i)
        pthread_create(&c[i], nullptr, producer, rq);
    for (int i = 0; i < 5; ++i)
        pthread_create(&s[i], nullptr, consumer, rq);

    for (int i = 0; i < 5; ++i)
        pthread_join(c[i], nullptr);
    for (int i = 0; i < 5; ++i)
        pthread_join(s[i], nullptr);
    delete rq;
    return 0;
}