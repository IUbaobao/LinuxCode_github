#include <iostream>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
using namespace std;

int main()
{
    int fd[2];
    int n = pipe(fd);
    assert(n == 0);
    (void)n;
    int id = fork();
    assert(id >= 0);
    if (id == 0)
    {
        // 子进程
        close(fd[1]); // 关闭写端
        int buff1[100];
        while (true)
        {
            ssize_t n = read(fd[0], buff1, sizeof(buff1) - 1);
            if (n == 0)
            {
                break;
            }
            printf("%s\n", buff1);
        }
        exit(0);
    }

    // 父进程
    close(fd[0]); // 关闭读端
    // 进程通信
    char buff[100];
    while (true)
    {
        cin >> buff;
        write(fd[1], buff, sizeof(buff));
    }
    // 进程等待
    int pid = waitpid(id, nullptr, 0);
    assert(pid == id);
    return 0;
}

// 进程间通信
/*
    测试代码
    让父进程写
    让子进程读
*/
// int main()
// {
//     int fd[2];
//     int n=pipe(fd);
//     assert(n==0);
//     (void)n;
//     int id=fork();
//     assert(id>=0);
//     if(id==0)
//     {
//         //子进程
//         close(fd[1]);//关闭写端
//         int buff1[100];
//         read(fd[0],buff1,sizeof(buff1)-1);
//         printf("%s\n",buff1);
//         exit(0);
//     }

//     //父进程
//     close(fd[0]);//关闭读端
//     //进程通信
//     char buff[100] = {"我是父进程，我要给子进程发信息!"};
//     write(fd[1],buff,sizeof(buff));
//     //进程等待
//     int pid = waitpid(id,nullptr,0);
//     assert(pid==id);
//     return 0;
// }