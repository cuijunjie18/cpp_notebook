#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
 
using namespace std;
 
int main() // 基于多路复用select函数实现的并行服务器
{
    // 1 创建监听的fd
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // 2 绑定
    struct sockaddr_in addr; // struct sockaddr_in是用于表示IPv4地址的结构体，它是基于struct sockaddr的扩展。
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9997);
    addr.sin_addr.s_addr = INADDR_ANY;
    (void)bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
 
    // 3 设置监听
    (void)listen(lfd, 128);
 
    // 将监听的fd的状态交给内核检测
    int maxfd = lfd;
    // 初始化检测的读集合
    fd_set rdset;
    fd_set rdtemp;
    // 清零
    FD_ZERO(&rdset);
    // 将监听的lfd设置到集合当中
    FD_SET(lfd, &rdset);
 
    // 通过select委托内核检测读集合中的文件描述符状态, 检测read缓冲区有没有数据
    // 如果有数据, select解除阻塞返回
    while (1)
    {
 
        rdtemp = rdset;
        int num = select(maxfd + 1, &rdtemp, NULL, NULL, NULL);
 
        // 判断连接请求还在不在里面，如果在，则运行accept
        if (FD_ISSET(lfd, &rdtemp))
        {
            struct sockaddr_in cliaddr;
            int cliaddrLen = sizeof(cliaddr);
            int cfd = accept(lfd, (struct sockaddr *)&cliaddr, (socklen_t *)&cliaddrLen);
 
            // 得到了有效的客户端文件描述符，将这个文件描述符放入读集合当中，并更新最大值
            FD_SET(cfd, &rdset);
            maxfd = cfd > maxfd ? cfd : maxfd;
        }
 
        // 如果没有建立新的连接，那么就直接通信
        for (int i = 0; i < maxfd + 1; i++)
        {
            if (i != lfd && FD_ISSET(i, &rdtemp))
            {
 
                // 接收数据，一次接收10个字节，客户端每次发送100个字节,下一轮select检测的时候, 内核还会标记这个文件描述符缓冲区有数据 -> 再读一次
                //  	循环会一直持续, 知道缓冲区数据被读完位置
                char buf[10] = {0};
                int len = read(i, buf, sizeof(buf));
                cout << "len=" <<len<< endl;
 
                if (len == 0) // 客户端关闭了连接，，因为如果正好读完，会在select过程中删除
                {
                    printf("客户端关闭了连接.....\n");
                    // 将该文件描述符从集合中删除
                    FD_CLR(i, &rdset);
                    close(i);
                }
                else if (len > 0) // 收到了数据
                {
                    // 发送数据
                    if (len > 2)
                    {
                        write(i, buf, strlen(buf) + 1);
                        cout << "写了一次" << endl;
                        sleep(1);
                    }
                }
                else
                {
                    // 异常
                    perror("read");
                    FD_CLR(i, &rdset);
                }
            }
        }
    }
 
    return 0;
}