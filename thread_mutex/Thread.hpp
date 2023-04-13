#pragma once

#include <pthread.h>
#include <functional>
#include <string>
#include <cassert>

class Thread;
// 线程上下文
struct Context
{
    Thread *_this;
    void *_args;
    Context() {}
    Context(Thread *this_, void *args = nullptr) : _this(this_), _args(args) {}
};

//////对原生线程库做一下封装
class Thread
{
public:
    typedef std::function<void *(void *)> func_t;
    int num = 1024;

public:
    Thread(func_t func, void *args = nullptr, int number = 0)
        : _func(func), _args(args)
    {
        char buff[num];
        snprintf(buff, sizeof buff, "thread->%d", number); // 给线程起个名称
        _name = buff;

        Context *ctx = new Context();
        ctx->_this = this;
        ctx->_args = args;
        int n = pthread_create(&_tid, nullptr, start_routine, (void *)ctx);
        // 异常 == if: 意料之外用异常或者if判断
        // assert: 意料之中用assert
        assert(n == 0);
        (void)n;
    }

    static void *start_routine(void *args)
    {
        Context *ctx = static_cast<Context *>(args);

        void *ret = ctx->_this->run(ctx->_args);
        delete ctx;
        return ret;
    }

    void *run(void *args)
    {
        return _func(args);
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
        (void)n;
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
};