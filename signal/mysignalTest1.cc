#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void handler(int signo)
{
    cout<<"捕捉到一个信号，信号编号是："<<signo<<endl;
}

int main()
{










    //这里是signal函数的调用，并不是handler函数调用
    //仅仅只是设置了对2号信号的捕捉方法，handler一般不会执行
    //除法收到对应信号
   // signal(2,handler);
   // 对全部1-31信号进行自定义捕捉方法
   //// 实验结果证明，并不是所有信号都能被用户自定义捕捉，9号信号就不能让用户自定义捕捉方式
   // for(int i=1;i<=31;i++)
   // {
   //     signal(i,handler);
   // }

   // while(1)
   // {
   //     cout<<"我是一个进程："<<getpid()<<endl;
   //     sleep(1);
   // }
    return 0;
}
