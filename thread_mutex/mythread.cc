#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include <string>
#include "Thread.hpp"
#include <memory>
using namespace std;

//int value = 100;

// 添加__thread，可以将一个内置类型设置为线程局部存储
// __thread int value = 100;

// struct apple
// {
//     int num=0;
//     double money=10;
//     //char buff[128];
//     //char a;
//     string s;
// };
//__thread apple a;//这个编译报错


// void *start_routine(void *arg)
// {
//     char *str = static_cast<char *>(arg);
//     while (1)
//     {
//         printf("我是%s......value:%d....&value:0x%p\n", str, value++, &value);
//         // printf("我是%s......apple->a:%d....&apple:0x%p\n", str, a.num++, &a);

//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, start_routine, (void *)"线程1");
//     while (1)
//     {
//         printf("我是主线程......value:%d....&value:0x%p\n", value, &value);
//         // printf("我是主线程......apple->a:%d....&apple:0x%p\n", a.num, &a);

//         sleep(1);
//     }
//     pthread_join(tid,nullptr);
//     return 0;
// }



// void* run(void* args)
// {
//     int cnt=3;
//     while(cnt--)
//     {
//         printf("%s.....正在运行\n",(char*)args);
//         sleep(1);
//     }
//     return nullptr;
// }



////////////////////////////////////////////////////////////////

#include "Mutex.hpp"
//初始化互斥锁有两种方式：
//1.静态分配
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;//--在全局定义的锁要用这个初始化,而且这个锁不用自己手动销毁

//2.动态分配--后面会经常用
int ticket=1000;

void* getTicket(void* args)
{
    char* username=static_cast<char*>(args);
    while(true)
    {
        // 加锁和解锁的过程多个线程串行执行的，程序变慢了！
        // 锁只规定互斥访问，没有规定必须让谁优先执行
        // pthread_mutex_lock(&mutex);
        //RAII风格的加锁
        LockGuard lockguard(&mutex);
        if(ticket>0)
        {
            usleep(1234);// 用这段时间来模拟真实的抢票要花费的时间
            cout<<username<<"正在抢票......"<<ticket<<endl;//最后票数可能小于0，原因多个线程并发执行出现了不安全的现象
            //此时需要加锁.....而这段要进行加锁保护的资源就叫临界资源，这个代码段就叫临界区
            ticket--;
            // pthread_mutex_unlock(&mutex);//这种解锁方式会很low，因为要在每一个可能退出的结果前都要写着一句代码
        }
        else
        {
            // pthread_mutex_unlock(&mutex);
            break;
        }
    }
    return nullptr;
}



int main()
{
    // Thread t1(run,(void*)"线程1",1);
    // Thread t2(run,(void*)"线程2",2);
    // Thread t3(run,(void*)"线程3",3);

    // t1.join();
    // t2.join();
    // t3.join();

    // cout<<"所有线程已正常退出"<<endl;
    //RAII 方式---智能指针
    std::unique_ptr<Thread>  thread1(new Thread(getTicket,(void*)"user1",1));
    std::unique_ptr<Thread>  thread2(new Thread(getTicket,(void*)"user2",2));
    std::unique_ptr<Thread>  thread3(new Thread(getTicket,(void*)"user3",3));
    std::unique_ptr<Thread>  thread4(new Thread(getTicket,(void*)"user4",4));

    
    thread1->join();
    thread2->join();
    thread3->join();
    thread4->join();

    return 0;
}