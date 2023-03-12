#pragma once 
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 0x66
// 共享内存的大小，一般建议是4KB的整数倍
// 系统分配共享内存是以4KB为单位的！ --- 内存划分内存块的基本单位Page
#define MAX_SIZE 4096 // --- 内核给你的会向上取整， 内核给你的，和你能用的，是两码事


key_t getkey()
{
    key_t key=ftok(PATHNAME,PROJ_ID);
    if(key<0)
    {
        std::cerr<<"getkey()"<<errno<<strerror(errno)<<std::endl;
        exit(1);
    }
    return key;
}


int getShmHelper(key_t key,int flags)
{
    // k是要shmget，设置进入共享内存属性中的！用来表示
    // 该共享内存，在内核中的唯一性！！
    // shmid vs key:
    // fd    vs inode
    int shmid=shmget(key,MAX_SIZE,flags);
    if(shmid<0)
    {
        std::cerr<<"getShmHelper"<<errno<<strerror(errno)<<std::endl;
        exit(2);
    }
    return shmid;
}

int createshm(key_t key)
{
    return getShmHelper(key,IPC_CREAT|IPC_EXCL | 0666);
}

int getshm(key_t key)
{
    return getShmHelper(key,IPC_CREAT);
}

void* attachShm(int shmid)
{
    void* mem=shmat(shmid,nullptr,0);//64系统，8
    if((long long)mem==-1L)
    {
        std::cerr<<"attachShm()"<<errno<<":"<<strerror(errno)<<std::endl;
        exit(3);
    }
    return mem;
}

void deltachShm(void* start)
{
    if(shmdt(start)==-1)
    {
        std::cerr<<"deltachShm()"<<":"<<errno<<strerror(errno)<<std::endl;
        exit(5);
    }
}


void delShm(int shmid)
{
    if(shmctl(shmid,IPC_RMID,nullptr)==-1)
    {
        std::cerr<<"delShm"<<errno<<":"<<strerror(errno)<<std::endl;
        exit(4);
    }
}













