#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "log.txt"

int main()
{
    //系统接口
    //O_WRONLY 只读 
    //O_CREAT 创建
    //O_TRUNC 清空文件内容
    //O_APPEND 追加
   //int fd=open(FILENAME,O_WRONLY | O_CREAT | O_TRUNC ,0666);//0666默认权限
  // int fd=open(FILENAME,O_WRONLY | O_CREAT | O_APPEND ,0666);//0666默认权限
  // if(fd<0)
  // {
  //     perror("open file:");
  // }
  //// const char*str1="aaaai\n";
  //  int cnt =5;
  //  while(cnt--)
  //  {
  //      write(fd,str1,strlen(str1));
  //  }

//   const char* str="hello linux\n";
//   int cnt =5;
//   while(cnt--)
//   {
//      write(fd,str,strlen(str)); 
//   }
//
   //O_RDONLY 只读
   //int fd=open(FILENAME,O_RDONLY);
   // char buff[1024];
   //read(fd,buff ,sizeof(buff));
   //printf("%s\n",buff);

  // close(fd);

   

    //C语言提供的文件接口
   //FILE* fp=fopen(FILENAME,"r");
   ////FILE* fp=fopen(FILENAME,"w");
   //assert(fp!=NULL);

   ////int cnt=5;
   ////const char* str="hello linux!\n";
   ////while(cnt--)
   ////{
   ////    fwrite(str,strlen(str),1,fp);
   ////}
   //char buff[1024];
   //while(fread(buff,sizeof(buff),1,fp));
   //printf("%s",buff);


   //fclose(fp);
  // int n=0;
  // printf("%d\n",n);
  // int id=fork();
  // if(id==0)
  // {
  //     printf("%d\n",n+1);
  // }
  
    //文件描述符
    //系统会默认帮我们打开三个标准流
    //0 stdin     标准输入  键盘
    //1 stdout    标准输出  显示器
    //2 stderror  标准错误  显示器
    //所以open一个文件打开之后一般都是从3开始



    return 0;
}
