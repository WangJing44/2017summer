/*************************************************************************
	> File Name: dual_pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月03日 星期四 21时37分23秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<wait.h>

/*子进程读写管道的函数*/
void child_rw_pipe(int readfd,int writefd)
{
    char *message1 = "from child process \n";
    write(writefd,message1,strlen(message1) +1);

    char message2[100];
    read(readfd,message2,100);
    printf("child process read from pipe :%s",message2);
}

/*父进程读写管道函数*/
void parent_rw_pipe(int readfd,int writefd)
{
    char *message1 = "from parent process !\n";
    write(writefd,message1,strlen(message1)+1);

    char message2[100];
    read(readfd,message2,100);
    printf("parent process read from pipe : %s",message2);
}

int main ()
{
    int pipe1[2],pipe2[2];
    pid_t pid;
    int sta_val;

    printf("realize full-duplex communication:\n\n");
    if(pipe(pipe1))
    {
        printf("pipe1 failed!\n");
        exit(1);
    }
    if(pipe(pipe2))
    {
        printf("pipe2 failed!\n");
        exit(1);
    }

    pid = fork();
    switch(pid)
    {
        case -1:
            printf("fork error!\n");
            exit(1);
        case 0:
            /*子进程关闭pipe1的写端，关闭pipe2的读段*/
            close(pipe1[1]);
            close(pipe2[0]);
            child_rw_pipe(pipe1[0] , pipe2[1]);
            exit(0);
        default:
            /*父进程关闭pipe2的写段，关闭pipe1的读段*/
            close(pipe2[1]);
            close(pipe1[0]);
            parent_rw_pipe(pipe2[0] , pipe1[1]);
            wait(&sta_val);
            exit(0);
    }
}
