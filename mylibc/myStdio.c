#include"myStdio.h"




FILE_* fopen_(const char* path_name,const char*mode)
{
    assert(path_name);
    int flag=0;
   if(strcmp(mode,"w")==0)
   {
        flag|=O_WRONLY | O_CREAT |O_TRUNC;
   }
   else if(strcmp(mode,"r")==0) 
   {
        flag |=O_RDONLY;
   }
   else if(strcmp(mode,"a")==0)
   {
        flag|=O_WRONLY | O_CREAT |O_APPEND;
   }
   else 
   {
        //其他情况
   }
   int fd=0;
   if(flag & O_RDONLY) fd=open(path_name,flag);
   else fd=open(path_name,flag,0666);
   if(fd<0)
   {
      const char* err= strerror(errno);
      write(2,err,strlen(err));
      return NULL; //这也是为什么打开文件失败会返回NULL
   }
   FILE_* fp=(FILE_*)malloc(sizeof(FILE_));
   assert(fp);
   fp->fileno=fd;
   fp->flags=SYNC_LINE; // 默认行缓冲
   fp->capacity=SIZE;
   fp->size=0;
   return fp;//这也是 为什么你们打开一个文件，就会返回一个FILE *指针
}


void fwrite_(const void* ptr,int num,FILE_* fp)
{
    //1.写入缓冲区
    memcpy(fp->buffer+fp->size,ptr,num);//暂时不考缓冲区溢出问题
    fp->size+=num;

    //2. 判断是否刷新
    if(fp->flags & SYNC_NOW)
    {
        write(fp->fileno,fp->buffer,fp->size);
        fp->size=0;
    }
    else if(fp->flags & SYNC_LINE)
    {
        //行缓冲 遇'\n'，就写入
        if(fp->buffer[fp->size-1]=='\n') //不考虑abc\abc这样的情况
        {
            write(fp->fileno,fp->buffer,fp->size);
            fp->size=0;
        }
    }
    else if(fp->flags & SYNC_FULL)
    {
        if(fp->size==fp->capacity)
        {
            write(fp->fileno,fp->buffer,fp->size);
            fp->size=0;
        }
    }
    else 
    {
        //.....
    }
}
void fclose_(FILE_*fp)
{
    //关闭文件前先刷新数据
    fflush_(fp);
    close(fp->fileno);
}
void fflush_(FILE_*fp)
{
    write(fp->fileno,fp->buffer,fp->size);
    fsync(fp->fileno); //将数据强制要求OS进行外设刷新！
    fp->size=0;

}
