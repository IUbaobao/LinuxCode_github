#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <cstdio>

using namespace std;

void Count(int cnt)
{
    while (cnt)
    {
        printf("cnt:%2d\r", cnt--);
        fflush(stdout);
        sleep(1);
    }
    cout << endl;
}

void handler(int signo)
{
    cout << "捕捉到一个信号：" << signo << endl; // 子进程退出会给父进程发信号--SIGCHLD(17)

    // 1.有可能同一时刻有非常多的子进程给父进程发信号,在同一时刻只有一部分退出
    while (1)
    {
        pid_t ret = waitpid(-1, NULL, WNOHANG); //-1表示等待任意一个子进程，WNOHANG非阻塞等待
        if (ret == 0)
            break; // 等不到就退出进行执行父进程的代码
        else if(ret==-1)
        {
            cout<<"ret==-1"<<endl;
            cout<<errno<<strerror(errno)<<endl;
            break;
        }
    }
}

int main()
{
    //signal(SIGCHLD, handler);
    signal(SIGCLD,SIG_IGN);//信号被忽略
    signal(SIGCHLD,SIG_DFL);//默认信号处理
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程

        printf("我是子进程,我要退出了,我的pid是:%d,我的父进程是:%d\n", getpid(),getppid());
        Count(5);
        exit(1);
    }
    int cnt = 8;
    while (cnt--)
    {
        // printf("%2d我是父进程,我正在运行,我的pid是:%d\n",cnt, getpid());
        sleep(1);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// int quit=0;
volatile int quit = 0; // volatile---保持内存可见性
// 例如下面的例子：main函数中没有对quit进行修改，那么编译器就可以会对其进行优化
// 把quit的值存在了寄存器中，这样就不用每一次都到内存中拿数据，这样做可以提升效率---g++编译的时候加参数 -O3
// 同样也有可能存在一种现象，用handler方法进行信号捕捉的时候对quit进行修改，确实是将0修改1到内存上
// 但是由于quit已经可能被编译器优化导致，直接在寄存器中拿数据，还是保存着之前的0，这样导致的现象就是下面while循环并不会退出
// 所以volatile就是防止这样的现象出现，用volatile修饰的变量就会老老实实的在内存中拿数据

// void handler(int signo)
// {
//     sleep(1);
//     cout<<quit;
//     quit=1;
//     cout<<"->"<<quit<<endl;
//     cout<<"quit已修改"<<endl;
// }

// int main()
// {
//     signal(2,handler);
//     while(!quit);
//     cout<<"我是正常推出......."<<endl;
//     return 0;
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void handler(int signo)
// {
//     cout<<"获取到一个信号："<<signo<<"正在处理中"<<endl;
//     Count(20);
// }

// int main()
// {
//     struct sigaction act,oact;
//     act.sa_flags=0;
//     act.sa_handler= handler;
//     sigemptyset(&act.sa_mask);
//     //sa_mask ---当我们正在处理某一个信号的时候，那么OS会自动帮我们屏蔽该信号的来临(在阻塞信号表中置1)
//     //           那么该信号就不会被重复抵达，防止大量重复信号的递归式调用，如果也顺便想屏蔽其他信号，
//     //           我们就可以往sa_mask中添加要屏蔽的信号
//     sigaddset(&act.sa_mask,2);//屏蔽信号2
//     sigaction(3,&act,&oact);

//     while(true) sleep(1);

//     return 0;
// }
