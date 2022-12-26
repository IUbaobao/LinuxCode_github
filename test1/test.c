#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main()
{
//    pid_ t waitpid(pid_t pid, int *status, int options);
//    返回值：
//    当正常返回的时候waitpid返回收集到的子进程的进程ID；
//    如果设置了选项WNOHANG,而调用中waitpid发现没有已退出的子进程可收集,则返回0；
//    如果调用中出错,则返回-1,这时errno会被设置成相应的值以指示错误所在
//    //测试能fork多少个子进程
    
    pid_t id=fork();
    if(id<0)
    {
        return 1;
    }
    else if(id==0)
    {
        //子进程
        sleep(2);
        //野指针错误
        int* p=NULL;
        *p=100;
        exit(1);
    }

    int status=0;
    int ret =waitpid(id,&status,0);
    if(id>0)
    {
        printf("child num:%dchild signal code:%d,child exit code:%d\n",id,status & 0x7F, (status>>8)& 0xFF);
    }
    return 0;


//   // int cnt=0;
   // while(1)
   // {
   //     pid_t id =fork();
   //     if(id<0)
   //     {
   //         printf("fork error num:%d\n",cnt);
   //         break;
   //     }
   //     else if(id==0)
   //     {
   //         while(1)
   //             sleep(1);
   //     }

   //     cnt++;

   // }
   // //char*env=getenv("HOME");
    //if(env)
    //{
    //  printf("%s\n",env);  
    //}
    return 0;
}
