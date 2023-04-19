#pragma once

#include "Thread.hpp"
#include "LockGuard.hpp"
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#define NUM 3

template <class T>
class ThreadPool;

template <class T>
struct ThreadData
{
    ThreadPool<T> *_threadpool;
    std::string _name;
    ThreadData(ThreadPool<T> *threadpool, const std::string &name) : _threadpool(threadpool), _name(name) {}
};

template <class T>
class ThreadPool
{
private:
    static void *handlerTask(void *args)
    {
        ThreadData<T> *td = static_cast<ThreadData<T> *>(args);
        while (true)
        {
            T t;
            {
                LockGuard lock(td->_threadpool->get_mutex()); // RAII方式的锁
                // td->_threadpool->lockqueue();
                while (td->_threadpool->isQueueEmpty())
                {
                    td->_threadpool->threadWait(); // 在条件变量下等待任务
                }
                // 拿任务
                t = td->_threadpool->pop();
                // td->_threadpool->unlockqueue();
            }
            // t();//解锁后再执行任务
            std::cout << td->_name << " 获取了一个任务： " << t.toSting() << " 并处理完成，结果是：" << t() << std::endl;
        }
        delete td;
        return nullptr;
    }

public:
    void lockqueue() { pthread_mutex_lock(&_mutex); }
    void unlockqueue() { pthread_mutex_unlock(&_mutex); }
    bool isQueueEmpty() { return _task_queue.empty(); }
    void threadWait() { pthread_cond_wait(&_cond, &_mutex); }
    pthread_mutex_t *get_mutex() { return &_mutex; }

public:
    void run() // 线程池全部线程启动
    {
        for (const auto &t : _threads)
        {
            ThreadData<T> *td = new ThreadData<T>(this, t->threadName());
            t->start(handlerTask, td);
            std::cout << t->threadName() << " : start......" << std::endl;
        }
    }
    T pop()
    {
        T t = _task_queue.front();
        _task_queue.pop();
        return t;
    }

    void push(const T &in) // push任务
    {
        LockGuard lock(&_mutex);
        // lockqueue();
        _task_queue.push(in);
        pthread_cond_signal(&_cond); // 唤醒线程
        // unlockqueue();
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        for (auto &t : _threads)
            delete t;
    }
    //单列模式
    void operator=(const ThreadPool<T>& ) =delete;//防止赋值
    ThreadPool(const ThreadPool<T>& t)= delete;//防止拷贝
public:
    static ThreadPool<T> *getInstance() // 单列模式---饿汉
    {
        if (tp==nullptr)
        {
            _singlock.lock();
            if (tp == nullptr)
            {
                tp = new ThreadPool<T>();
            }
            _singlock.unlock();
        }
        return tp;
    }

private:
    ThreadPool(const int &num = NUM) : _num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 0; i < _num; ++i)
        {
            _threads.push_back(new Thread());
        }
    }

private:
    int _num;
    std::vector<Thread *> _threads;
    std::queue<T> _task_queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    static ThreadPool<T> *tp;
    static std::mutex _singlock;
};

template <class T>
ThreadPool<T> *ThreadPool<T>::tp = nullptr;

template <class T>
std::mutex ThreadPool<T>::_singlock;