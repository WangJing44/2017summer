/*************************************************************************
	> File Name: my_recv.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月07日 星期一 11时22分33秒
 ************************************************************************/
#define MY_RECV_C


#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"my_recv.h"

/*自定义的错误处理函数*/
void my_err(const char *err_string , int line)
{
    fprintf(stderr , "line:%d ",line);
    perror(err_string);
    exit(1);
}

int my_recv(int conn_fd , char *data_buf , int len)
{
    static char recv_buf[BUFSIZE];
    static char *pread;//指向下一次读取数据的位置
    static int len_remain = 0 ;//自定义缓冲区中剩余的字数
    int i;
    //如果自定义缓冲区中没有数据，则从套接字读取数据
    if(len_remain <= 0)
    {
        if((len_remain = recv(conn_fd , recv_buf , sizeof(recv_buf), 0)) < 0)
            my_err("recv" , __LINE__);
        else if(len_remain == 0) //目的计算机的socket连接关闭
            return 0;
        pread = recv_buf;//初始化pread指针
    }
    //自定义缓冲区读取一次的数据
    for(i = 0 ; *pread != '\n' ; i++)
    {
        if(i > len)//防止指针越界
        {
            return -1;
        }
        data_buf[i] = *pread++;
        len_remain--;
    }
    //去除结束标志
    len_remain--;
    pread++;

    return i;
}
