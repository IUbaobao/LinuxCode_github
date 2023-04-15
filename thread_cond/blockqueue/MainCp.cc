#include  <iostream>
#include "BlockQueue.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include "Task.hpp"
#include <vector>

using namespace std;

template<class T>
struct thread_data
{
    BlockQueue<T>* _bq;
    std::string _name;
    pthread_t _tid;
    thread_data(){}
    thread_data(BlockQueue<T>* bq,std::string name)
    :_bq(bq),_name(name){}
};

//C:计算队列
//S: 保存队列
template<class C,class S>
struct  BlockQueues
{
    BlockQueue<C> c_bq;
    BlockQueue<S> s_bq;
};


void* producter(void* args)
{
    //生产者
    // BlockQueue<int>* bq=static_cast<BlockQueue<int>*>(args);
    // BlockQueue<CallTask>*  bq=static_cast<BlockQueue<CallTask>* >(args);
    // thread_data<CallTask>*t_data=static_cast<thread_data<CallTask>* >(args);
    BlockQueues<CallTask,SaveTask>*bqs=static_cast<BlockQueues<CallTask,SaveTask>* >(args);


    while(true)
    {
        int x=rand()%10+1;
        int y=rand()%5;
        char operCode =oper[rand()%oper.size()];
        CallTask t(x,y,operCode,mymath);
        bqs->c_bq.push(t);
        // cout<<"生产者：生产了一个："<<t.toSting()<<endl;
        // cout<<t_data->_name<<"生产者：生产了一个："<<t.toSting()<<endl;
        cout<<"生产者：生产了一个："<<t.toSting()<<endl;
        sleep(1);
    }
    return nullptr;
}

void* consumer(void* args)
{
    //消费者
    // BlockQueue<int>* bq=static_cast<BlockQueue<int>*>(args);
    // thread_data<CallTask>*t_data=static_cast<thread_data<CallTask>* >(args);
    BlockQueues<CallTask,SaveTask>*bqs=static_cast<BlockQueues<CallTask,SaveTask>* >(args);

    while(true)
    {
        CallTask t;
        bqs->c_bq.pop(&t);
        std::string str=t();
        cout<<"消费者：消费了一个:"<<str<<endl;//即是消费者也是生产者---消费生产者生产的计算任务，同时计算完成后自己再生产一个保存任务
        SaveTask s(str,save);
        bqs->s_bq.push(s);
        cout<<"生产者：生产了一个保存任务"<<endl;
        sleep(1);
    }
    return nullptr;
}

void* saver(void* args)
{
    BlockQueues<CallTask,SaveTask>*bqs=static_cast<BlockQueues<CallTask,SaveTask>* >(args);
    while(true)
    {
        SaveTask s;
        bqs->s_bq.pop(&s);
        s();
        cout<<"消费者:消费了一个保存任务"<<endl;
    }
}


int main()
{
    srand((unsigned long)time(nullptr)^getpid());
    // BlockQueue<int>* bq=new BlockQueue<int>(5);
    
    // BlockQueue<CallTask>* bq=new BlockQueue<CallTask>(5);
    BlockQueues<CallTask,SaveTask> bqs;
    //单生产单消费
    
    //生产者(生产计算任务)-----(c_bq阻塞队列)----消费者(完成计算)&&生产者(生产保存任务)------(s_bq阻塞队列)------消费者(完成保存)
    pthread_t p,c,s;
    pthread_create(&p,nullptr,producter,&bqs);
    pthread_create(&c,nullptr,consumer,&bqs);
    pthread_create(&s,nullptr,saver,&bqs);



    pthread_join(p,nullptr);
    pthread_join(c,nullptr);
    pthread_join(s,nullptr);



    // pthread_t pd,cs;
    // pthread_create(&pd,nullptr,producter,bq);
    // pthread_create(&cs,nullptr,consumer,bq);

    // pthread_join(pd,nullptr);
    // pthread_join(cs,nullptr);

    //多生产多消费
// #define NUM 5
//     vector<thread_data<CallTask>*> pds(NUM);
//     vector<thread_data<CallTask>*> css(NUM);
//     for(int i=0;i<NUM;++i)
//     {
//         pds[i]=new thread_data<CallTask>;
//         css[i]=new thread_data<CallTask>;
//         pds[i]->_bq=bq;
//         css[i]->_bq=bq;
//         char buff1[128];
//         char buff2[128];
//         snprintf(buff1,sizeof buff1,"producter thread %d",i+1);
//         snprintf(buff2,sizeof buff2,"consumer thread %d",i+1);
//         pds[i]->_name=buff1;
//         css[i]->_name=buff2;
//         pthread_create(&pds[i]->_tid,nullptr,producter,pds[i]);
//         pthread_create(&css[i]->_tid,nullptr,consumer,css[i]);
//     }

//     for(int i=0;i<NUM;++i)
//     {
//         pthread_join(pds[i]->_tid,nullptr);
//         pthread_join(css[i]->_tid,nullptr);
//         delete pds[i];
//         delete css[i];
//     }
    // delete bq;
    return 0;
}


