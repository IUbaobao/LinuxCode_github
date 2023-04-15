#pragma once
#include <iostream>
#include <pthread.h>
#include <queue>

#define MAXCAP 100
template <class T>
class BlockQueue
{
public:
    BlockQueue(const int &maxcap=MAXCAP)
        : _maxcap(maxcap)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_pcond, nullptr);
        pthread_cond_init(&_ccond, nullptr);
    }
    void push(const T &in) // 输入型参数，const &
    {
        pthread_mutex_lock(&_mutex);
        while (is_full()) // 队列为满的时候要在条件变量下阻塞等待
        {
            // 细节1：这里要用while，不能用if，因为有可以存在伪唤醒,又或者wait函数调用失败
            // 细节2：pthread_cond_wait这个函数的第二个参数，必须是我们正在使用的互斥锁！
            // a. pthread_cond_wait: 该函数调用的时候，会以原子性的方式，将锁释放，并将自己挂起
            // b. pthread_cond_wait: 该函数在被唤醒返回的时候，会自动的重新获取你传入的锁
            pthread_cond_wait(&_pcond, &_mutex);
        }

        _q.push(in);
        // 来到这里，阻塞队列中一定至少存在一个数据--这个时候可以通知生产者
        pthread_cond_signal(&_ccond); // 细节3：pthread_cond_signal：这个函数，可以放在临界区内部，也可以放在外部
        pthread_mutex_unlock(&_mutex);
        // pthread_cond_signal(&_ccond); 
    }
    void pop(T *out) // 输出型参数：*， // 输入输出型：&
    {
        pthread_mutex_lock(&_mutex);
        while (is_empty()) // 为空的时候，pop也要在条件变量下等待,此处也是while，道理同上
        {
            pthread_cond_wait(&_ccond, &_mutex);
        }
        // 来到这里，阻塞队列中一定至少存在一个数据--这时候可以pop
        *out = _q.front();
        _q.pop();
        // 来到这里，阻塞队列一定至少存在一个空位--这时候可以通知产生者
        pthread_cond_signal(&_pcond);
        pthread_mutex_unlock(&_mutex);
        // pthread_cond_signal(&_pcond);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_pcond);
        pthread_cond_destroy(&_ccond);
    }

private:
    bool is_full()
    {
        return _q.size() == _maxcap;
    }
    bool is_empty()
    {
        return _q.empty();
    }

private:
    std::queue<T> _q;
    int _maxcap; // 队列元素最大上限
    pthread_mutex_t _mutex;
    pthread_cond_t _pcond; // 生产者对应的条件变量
    pthread_cond_t _ccond; // 生产者对应的条件变量
};