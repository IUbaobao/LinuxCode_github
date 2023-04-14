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
            pthread_cond_wait(&_pcond, &_mutex);
        }

        _q.push(in);
        // 来到这里，阻塞队列中一定至少存在一个数据--这个时候可以通知生产者
        pthread_cond_signal(&_ccond);
        pthread_mutex_unlock(&_mutex);
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