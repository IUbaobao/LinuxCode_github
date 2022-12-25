#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
//
//int main(int argc ,char* argv[],char* env[])
//{
    //for(int i=0;i<argc;i++)
    //{
    //    printf("%s\n",argv[i]);
    //}
#define N 256

int main()
{

    printf("%s\n",getenv("MYVAL1"));
    //extern char** environ;
    //int i=0;
    //for(i=0;environ[i];++i)
    //{
    //    printf("%s\n",environ[i]);
    //}





//    while(1)
//    {
//    char string[N]; 
//    char* s=fgets(string,sizeof(string)-1,stdin);
//    assert(s!=NULL);
//    char* myargv[N];
//    //printf("%s\n",string);
//    string[strlen(string)-1]=0;
//    myargv[0]=strtok(string," ");
//    int i=1;
//    while(myargv[i++]=strtok(NULL," "));
//
//   // for(i=0;myargv[i];++i)
//   // {
//   //     printf("%s\n",myargv[i]);
//   // }
//    pid_t id= fork();
//    assert(id>=0);
//
//    if(id==0)
//    {
//        execvp(myargv[0],myargv);
//        _exit(1);
//    }
//

    //for(int i=0;env[i];i++)
    //{
    //    printf("%s\n",env[i]);
    //}

   // pid_t id =fork();
   // assert(id>=0);
   // if(id==0)
   // {
   //    
   //     //execl("/usr/bin/ls","ls","-l","-a",NULL);
   //     //execlp("ls","ls","-al",NULL);
   //     
   //    // printf("我是子进程\n");
   //    // //int i=1/0;
   //    _exit(2);
   // }
 //    int status=0;
 //    pid_t pid=waitpid(id,&status,0);
 //  if(pid!=-1)
 //  {
 //      printf("signal:%d,eixt code:%d\n",status&0x7F,(status>>8)& 0xFF);
 //  }
 // // }
    return 0;
}
