/* File Name: server.c */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define DEFAULT_PORT 8000
#define MAXLINE 4096
#define MAXLINE 4096

/* 服务器端通信流程
 * 1. 创建socket
 * 2. 填充&servaddr
 * 3. bind服务器的ip和端口到socket上
 * 4. 在指定端口上监听客户端是否连接
 * 5. 阻塞，接受客户端连接，创建连接socket，供连接的客户端和服务器通信
 * 6. 数据读写
 * 7. 通信结束后，断开连接socket
 * 8. 接受下一个客户端的连接*/

int main(int argc, char** argv)
{
    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr;  // 服务器的地址相关信息
    char    buff[4096];
    int     n;

    struct sockaddr_in client_addr;
    int length = sizeof(client_addr);

    //初始化Socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  // tcp
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    //初始化 //填充结构体:servaddr,把主机序转化为网络序。
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT

    //将本地(服务器地址)地址绑定到所创建的套接字上
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
    printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    //开始监听是否有客户端连接
    if( listen(socket_fd, 10) == -1){
    printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    printf("======waiting for client's request======\n");
    while(1){
//阻塞直到有客户端连接，不然多浪费CPU资源。 //客户端连接后，会为每一个连接服务器的客户端创建一个连接socket，后续的
//数据读写均在这个连接socket上进行, //客户端连接后，会为每一个连接服务器的客户端创建一个连接socket，后续的
//数据读写均在这个连接socket上进行，数据处理结束后，关闭此连接socket。
     	//if((connect_fd = accept(socket_fd,, (struct sockaddr*)&client_addr, &length)) == -1){
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){ //accept函数中包含客户端的地址相关信息，如果不关心客户端情况，则可以置为NULL，正常情况下，可以从中获取客户端的地址信息。
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        continue;
    }
//接受客户端传过来的数据
    n = recv(connect_fd, buff, MAXLINE, 0);
//向客户端发送回应数据
    if(!fork()){ /*紫禁城*/
        if(send(connect_fd, "Hello,you are connected!\n", 26,0) == -1)
        perror("send error");
        close(connect_fd);
        exit(0);
    }
    buff[n] = '\0';
    printf("recv msg from client: %s\n", buff);
    close(connect_fd);
    }
    close(socket_fd);
}
