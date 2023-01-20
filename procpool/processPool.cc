#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROCESS_NUM 5
#define SrandSeed() srand((unsigned)time(nullptr) ^ getpid() ^ rand() % 1234) // 随机数种子
typedef void (*fun_t)();

////////////////////////////模拟任务////////////////////////
void downLoadTask()
{
    std::cout << getpid() << ":下载任务!" << std::endl;
}

void ioTask()
{
    std::cout << getpid() << ":io任务!" << std::endl;
}

void flushTask()
{
    std::cout << getpid() << ":刷新任务!" << std::endl;
}

void loadTaskFunc(std::vector<fun_t> &funMap)
{
    funMap.push_back(downLoadTask);
    funMap.push_back(ioTask);
    funMap.push_back(flushTask);
}

///////////////////进程池///////////////////////////
class subEp // endpoint
{
public:
    subEp(int writeFd, pid_t subID)
        : _writeFd(writeFd), _subID(subID)
    {
        char namebuff[1024];
        snprintf(namebuff, sizeof namebuff, "process->%d[pid(%d)-fd(%d)]", num++, _subID, _writeFd);
        _name = namebuff;
    }

public:
    std::string _name;
    int _writeFd; // 管道写端的文件描述符
    pid_t _subID; // 子进程ID
    static int num;
};

int subEp::num = 0;

int main()
{
    SrandSeed();
    // 1. 建立子进程和子进程通信的管道
    std::vector<subEp> subs;
    std::vector<fun_t> funMap;
    // 加载任务
    loadTaskFunc(funMap);
    for (int i = 0; i < PROCESS_NUM; ++i)
    {
        // 1.1 创建管道
        int fds[2];
        int n = pipe(fds);
        assert(n == 0);
        (void)n;
        // 1.2 创建子进程
        pid_t id = fork();
        assert(id >= 0);
        if (id == 0)
        {
            // 子进程
            close(fds[1]); // 关闭写端
            while (true)
            {
                int taskNum;
                int n = read(fds[0], &taskNum, sizeof(taskNum));
                // 执行任务
                if (n == 0)
                {
                    break;
                }
                funMap[taskNum]();

            }
            exit(1);
        }
        // 父进程关闭读端
        close(fds[0]);
        subEp sub(fds[1], id);
        subs.push_back(sub);
    }

    // 2. 控制子进程，负载均衡的分配任务给子进程
    int subSize = subs.size();
    int TaskSize = funMap.size();
    int TaskCount = 5;
    for (int i = 0; i < TaskCount; i++)
    {
        // 随机选一个子进程
        int subNum = rand() % subSize;
        // 随机选一个任务
        int TaskNum = rand() % TaskSize;
        // 发送任务
        write(subs[subNum]._writeFd,&TaskNum,sizeof(TaskNum));
        sleep(1);
    }
    for(int i=0;i<PROCESS_NUM;i++)
    {
        close(subs[i]._writeFd);
    }
    // 3. 回收子进程
    for(int i=0;i<PROCESS_NUM;++i)
    {
        waitpid(subs[i]._subID,nullptr,0);
    }
    return 0;
}