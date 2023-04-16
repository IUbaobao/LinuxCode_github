#pragma once

#include <semaphore.h>
#include <vector>
#include <cassert>
#include <pthread.h>
template <class T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        int n=sem_wait(&sem);
        assert(n==0);
        (void)n;
    }

    void V(sem_t &sem)
    {
        int n=sem_post(&sem);
        assert(n==0);
        (void)n;
    }
public:
    RingQueue(const int &cap) : _queue(cap), _cap(cap)
    {
        int n = sem_init(&_spaceSem, 0, _cap);
        assert(n == 0);
        n = sem_init(&_dataSem, 0, 0);
        assert(n == 0);

        _producerStep=_consumerStep=0;
        pthread_mutex_init(&_pmutex,nullptr);
        pthread_mutex_init(&_cmutex,nullptr);
    }

    void push(const T& in)
    {
        P(_spaceSem);// 申请到了空间信号量，意味着，一定能进行正常的生产
        pthread_mutex_lock(&_pmutex);
        _queue[_producerStep++]=in;
        _producerStep%=_cap;//循环队列模拟的方法
        pthread_mutex_unlock(&_pmutex);
        V(_dataSem);
    }

    void pop(T* out)
    {
        // 你认为：先加锁，后申请信号量，还是现申请信号量，在加锁？---先申请信号量....
        P(_dataSem);
        pthread_mutex_lock(&_cmutex);
        *out=_queue[_consumerStep++];
        _consumerStep%=_cap;
        pthread_mutex_unlock(&_cmutex);
        V(_spaceSem);
    }

    ~RingQueue()
    {
        sem_destroy(&_spaceSem);
        sem_destroy(&_dataSem);
        pthread_mutex_destroy(&_pmutex);
        pthread_mutex_destroy(&_cmutex);
    }

private:
    std::vector<T> _queue;
    int _cap;
    sem_t _spaceSem; // 生产者，要生产，看中的是什么资源? 空间资源
    sem_t _dataSem;  // 消费者，要消费，看中的是什么资源？ 数据资源
    int _producerStep;
    int _consumerStep;
    pthread_mutex_t _pmutex;
    pthread_mutex_t _cmutex;
    
};