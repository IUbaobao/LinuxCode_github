#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <vector>
using namespace std;

// int sigemptyset(sigset_t *set);初始化set所指向的信号集
// int sigfillset(sigset_t *set);函数sigfillset初始化set所指向的信号集
// int sigaddset (sigset_t *set, int signo);
// int sigdelset(sigset_t *set, int signo);
// int sigismember（const sigset_t *set, int signo);
//sigismember是一个布尔函数,用于判断一个信号集的有效信号中是否包含某种 信号,若包含则返回1,不包含则返回0,出错返回-1。


// 实际执行信号的处理动作称为信号递达(Delivery)
// 信号从产生到递达之间的状态,称为信号未决(Pending)。
// 进程可以选择阻塞 (Block )某个信号
// 被阻塞的信号产生时将保持在未决状态,直到进程解除对此信号的阻塞,才执行递达的动作.
// 注意,阻塞和忽略是不同的,只要信号被阻塞就不会递达,而忽略是在递达之后可选的一种处理动作。

#define MAX_SIGNUM 31

vector<int> sigarr={2};


void show_pending(sigset_t pending)
{
    for(int signo=MAX_SIGNUM;signo>=1;--signo)
    {
        if(sigismember(&pending,signo))
            cout<<"1";
        else
            cout<<"0";
    }
    cout<<endl;
}

int main()
{
    //先尝试屏蔽指定的信号
    sigset_t block,oblock,pending;
    //初始化
    sigemptyset(&block);
    sigemptyset(&oblock);
    //添加要屏蔽的信号
    // for(auto e: sigarr) sigaddset(&block,e);
    for(int signo=1;signo<MAX_SIGNUM;++signo) sigaddset(&block,signo);//对信号9是无法进行屏蔽的
    //开始屏蔽，设置进内核中
    sigprocmask(SIG_BLOCK,&block,&oblock);
    //打印pending表
    int cnt=10;
    while(true)
    {
        //初始化
        sigemptyset(&pending);
        //获取内核中的block表
        sigpending(&pending);
        //打印
        show_pending(pending);
        sleep(1);
        if(cnt--==0)
        {

            // 一旦对特定信号进行解除屏蔽，一般OS要至少立马递达一个信号！
            //也就是说如果信号集中有接收到信号，解除屏蔽后，OS就会马上将信号递达到进程中
            //最后只会打印第一条信息，因为在打印第2条信息前进程已经执行该信号--退出
            cout << "1.恢复对信号的屏蔽，不屏蔽任何信号\n";
            sigprocmask(SIG_SETMASK,&oblock,&block); 
            cout << "2.恢复对信号的屏蔽，不屏蔽任何信号\n";
        }
    }
    return 0;
}
