#pragma once

#include <pthread.h>
#include <functional>
#include <string>
#include <cassert>

typedef std::function<void *(void *)> func_t;
int num = 1024;

//////对原生线程库做一下封装
class Thread
{
public:
    Thread()
    {
        char namebuff[num];
        snprintf(namebuff, sizeof namebuff, "thread->%d", number++); // 给线程起个名称
        _name = namebuff;
    }

    void start(func_t func, void *args = nullptr)
    {
        _func = func;
        _args = args;
        int n = pthread_create(&_tid, nullptr, start_routine, this);
        // 异常 == if: 意料之外用异常或者if判断
        // assert: 意料之中用assert
        assert(n == 0);
        (void)n;
    }

    static void *start_routine(void *args)
    {
        Thread *_this = static_cast<Thread *>(args);
        return _this->run();
    }

    void *run()
    {
        return _func(_args);
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
        (void)n;
    }
    std::string threadName()
    {
        return _name;
    }
    ~Thread()
    {
        // do nothing
    }

private:
    std::string _name; // 线程名称
    func_t _func;      // 线程执行函数
    pthread_t _tid;    // 线程tid
    void *_args;       // 传给线程的参数

    static int number;
};
int Thread::number = 1;