#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cstring>
using namespace std;

struct thread_data
{
    int _num;
    pthread_t _tid;
    char _namebuff[64];
    thread_data() {}
    thread_data(int num) : _num(num) {}
};

void *start_routine(void *arg)
{
    int cnt = 8;
    // char* str=static_cast<char*>(arg);//进行安全的类型转化
    thread_data *td = (thread_data *)arg;

    while (cnt--)
    {
        // cout<<str<<"正在运行...."<<endl;
        cout << td->_namebuff << "正在运行...." << endl;
        sleep(1);
        // int a=1/0;//一个线程发生异常会不会影响其他线程？----会（健壮性或者鲁棒性较差）
        // 因为发生异常可能会产生进程信号,信号是发给进程的！
    }
    // 怎么样可以结束一个线程呢？
    // exit(1);//可以用这些接口吗---不可以，因为exit是用来结束进程的，任何一个执行流调用exit都会让整个进程退出
    // char ret[128];
    // sprintf(ret, "%s->正常退出......", td->_namebuff);

    // 线程如何退出的问题
    // 1.调用接口
    // pthread_exit((void *)ret); // 该参数是返回给创建该线程的那个执行流
    // 2.线程函数结束，return的时候，线程就算终止了
    // return (void*)ret;
    // return nullptr;
    return (void *)888;
    // 既然一个整形变量都可以传递出去，那么堆空间的地址也同样可以可以
}

// int main()
// {
//     // 创建一批线程
//     vector<thread_data *> threads;
// #define NUM 10
//     for (int i = 0; i < NUM; ++i)
//     {
//         thread_data *td = new thread_data(i + 1);
//         sprintf(td->_namebuff, "%s:%d", "thread", i + 1);
//         pthread_create(&td->_tid, nullptr, start_routine, td);
//         threads.push_back(td);
//     }

//     // int pthread_cancel(pthread_t thread)----取消一个执行中的线程
//     // 线程是可以被cancel取消的！注意：线程要被取消，前提是这个线程已经跑起来了
//     // 线程如果是被取消的，退出码：-1
//     // PTHREAD_CANCELED;
//     sleep(2);
//     for (int i = 0; i < 5; ++i)
//     {
//         int cancel_ret = pthread_cancel(threads[i]->_tid);
//         if (cancel_ret == 0)
//         {
//             cout << threads[i]->_namebuff << "取消线程成功" << endl;
//         }
//     }

//     // 进程有进程等待，同样线程也用线程等待
//     // 为什么需要线程等待
//     //  已经退出的线程，其空间没有被释放，仍然在进程的地址空间内。--防止内存泄漏
//     //  获取新线程的退出信息
//     for (auto e : threads)
//     {
//         void *ret;
//         int join_ret = pthread_join(e->_tid, &ret); // 等待线程结束，第一个参数是线程id，第二参数接收线程的返回值，不关心线程返回值可以用nullptr
//         assert(join_ret == 0);
//         cout << e->_namebuff << "已退出,exit_code: " << (long long)ret << endl;//用pthread_cancel函数取消的线程返回的是-1
//         delete e;
//     }

//     // pthread_t tid;
//     // pthread_create(&tid,nullptr,start_routine,(void*)"线程1");

//     // void* ret;
//     // pthread_join(tid,&ret);
//     // char* str=static_cast<char*>(ret);
//     // cout<<str<<endl;
//     return 0;
// }

//////////分离线程
// 默认情况下，新创建的线程是joinable的，线程退出后，需要对其进行pthread_join操作，否则无法释放资源，从而造成系统泄漏。
// 如果不关心线程的返回值，join是一种负担，这个时候，我们可以告诉系统，当线程退出时，自动释放线程资源。
//int pthread_detach(pthread_t thread);

void* run(void* arg)
{
    char* str=static_cast<char*>(arg);
    int cnt=5;
    while(cnt--)
    {
        cout<<str<<"正在运行....."<<endl;
        sleep(1);
        //可以在新线程中分离
        //pthread_detach(pthread_self());
    }
    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,run,(void*)"线程1");

    //在主线程的分离该线程
    pthread_detach(tid);
    sleep(1);//看到的现象是：新线程运行运行1s后，整个进程退出
    cout<<"主线程已正常退出"<<endl;

    //正常情况要等待
    // sleep(2);
    // int ret=pthread_join(tid,nullptr);
    // //assert(ret==0);
    // cout<<ret<<":"<<strerror(ret)<<endl;//线程已经分离还等待报错---22:Invalid argument
    return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////
// 重新认识pthread原生库
// #include <thread>
// void start_run()
// {
//     while(1)
//     {
//         cout<<"我是一个线程"<<endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     // 任何语言，在linux中如果要实现多线程，必定要是用pthread库
//     // 如何看待C++11中的多线程呢？？C++11 的多线程，在Linux环境中，本质是对pthread库的封装！
//     // std::thread t1([]()
//     //                {
//     //                    while (true)
//     //                    {
//     //                        cout << "我是一个线程" << endl;
//     //                        sleep(1);
//     //                    }
//     //                });
//     std::thread t1(start_run);
//     while (true)
//     {
//         cout << "我是主线程........." << endl;
//         sleep(1);
//     }
//     t1.join();
//     return 0;
// }
