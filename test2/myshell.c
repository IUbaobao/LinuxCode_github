#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM 1024
#define OPT_NUM 64

char lineCommand[NUM];
char* myargv[OPT_NUM];
int exitCode=0;
int signalCode=0;

int main()
{
    
    while(1)
    {
         char* hostname=getenv("HOSTNAME");
         char* user=getenv("USER");

         //打印提示符
         printf("[%s@%s 当前路径]# ",hostname,user);
         fflush(stdout);
         //接收用户命令
         char* s=fgets(lineCommand,sizeof(lineCommand)-1,stdin);
         assert(s!=NULL);
         //清除换行符\n
         lineCommand[strlen(lineCommand)-1]=0;
         //printf("%s\n",lineCommand);
         //解析命令
         myargv[0]=strtok(lineCommand," ");
         int i=1;
         
         //对ls添加颜色
         if(strcmp(myargv[0],"ls")==0)
         {
             myargv[i++]=(char*)"--color=auto";
         }

         while(myargv[i++]=strtok(NULL," "));

        //内建命令：不需要通过子进程来完成任务，而是让shell本身去做的命令
        //对cd做处理
        if(strcmp(myargv[0],"cd")==0)
        {
            if(myargv[1]!=NULL)
            {
                chdir(myargv[1]);
            }
            continue;

        }
        //对echo做处理
        if(strcmp(myargv[0],"echo")==0)
        {
            if(myargv[1]!=NULL &&  strcmp(myargv[1],"$?")==0)
            {
                printf("signalCode:%d,exitCode:%d\n",signalCode,exitCode);
            }
            else 
            {
                for(int j=1;myargv[j];j++)
                {
                    printf("%s ",myargv[j]);
                }
                printf("\n");
            }
            continue;
        }


//     条件编译
#ifdef DEBUG 
         for(int i=0;myargv[i];i++)
         {
             printf("%s\n",myargv[i]);
         }
#endif 

         pid_t id = fork();
         assert(id!=-1);
         if(id==0)
         {
             execvp(myargv[0],myargv);
             exit(1);
         }
         int status=0;
         //0表示阻塞等待
         int ret=waitpid(id,&status,0);
         exitCode=(status>>8)& 0xFF;
         signalCode=status & 0x7F;
    }







    return 0;
}
