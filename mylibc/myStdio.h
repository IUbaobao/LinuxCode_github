#pragma once 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1024
#define SYNC_NOW  1 //立刻刷新，无缓冲
#define SYNC_LINE 2 //行缓冲
#define SYNC_FULL 3 //全缓冲

typedef struct _FILE
{
    int flags;//刷新方式
    int fileno;
    int size;
    int capacity;
    char buffer[SIZE];

}FILE_;


FILE_* fopen_(const char* path_name,const char*mode);
void fwrite_(const void* ptr,int num,FILE_* fp);
void fclose_(FILE_*fp);
void fflush_(FILE_*fp);
