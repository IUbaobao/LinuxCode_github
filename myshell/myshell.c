#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>


#define NUM 1024
#define OPT_NUM 64
//清除空格
#define trimSpace(start) do{\
    while(isspace(*start)) ++start;\
    }while(0)




#define NONE_REDIR   0 //没有重定向
#define INPUT_REDIR  1 //输入重定向
#define OUTPUT_REDIR 2 //输出重定向
#define APPEND_REDIR 3 //追加重定向


int redirType=NONE_REDIR;
char* redirfile=NULL;


char lineCommand[NUM];
char* myargv[OPT_NUM];
int exitCode=0;
int signalCode=0;


void checkcommand(char* command)
{
    assert(command);
    char* start=command;
    char* end=command+strlen(command);
    while(start<end)
    {
        if(*start=='>')
        {
            *start='\0';
            start++;
            if(*start=='>')
            {
                redirType=APPEND_REDIR;
                start++;
            }
            else
            {
                redirType=OUTPUT_REDIR;
            }
            trimSpace(start);
            redirfile=start;
            break;
        }
        else if(*start=='<') 
        {
            *start='\0';
            start++;
            //填写重定向信息
            redirType=INPUT_REDIR;
            trimSpace(start);
            redirfile=start;
            break;
        }
        else 
        {
            ++start;
        }

    }
}


int main()
{
    
    while(1)
    {

        //初始化重定向信息
         redirType=NONE_REDIR;
         redirfile=NULL;
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
         
         checkcommand(lineCommand);
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
             switch(redirType)
             {
                 case NONE_REDIR:
                     //什么都不做
                     break;
                case OUTPUT_REDIR:
                case APPEND_REDIR:
                     {
                         umask(0);
                         int flags=O_WRONLY | O_CREAT;
                         if(redirType==APPEND_REDIR)
                         {
                            //追加重定向
                            flags |=O_APPEND;
                         }
                         else 
                         {
                             //写入+清空原本内容
                             flags |=O_TRUNC;
                         }
                         int fd= open(redirfile,flags,0666);
                         if(fd<0)
                         {
                             perror("open file:");
                             exit(2);
                         }
                         dup2(fd,1);
                         break;
                     } 
                case INPUT_REDIR:
                     {
                         int fd=open(redirfile,O_RDONLY);
                         if(fd<0)
                         {
                             perror("open file:");
                             exit(1);
                         }
                         //更改文件描述符指向---》重定向
                         dup2(fd,0);
                         break;
                     }
                default:
                     printf("bug?\n");
                     break;
             }
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
