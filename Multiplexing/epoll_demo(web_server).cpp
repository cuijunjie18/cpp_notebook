#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8888
#define MAX_EVENTS 1024  // epoll 最大监听事件数
#define BUFFER_SIZE 1024

// 设置 socket 为非阻塞模式
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    
    // 1. 创建服务器 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 2. 设置 socket 选项
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 3. 绑定地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 4. 监听连接
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d (epoll mode)\n", PORT);

    // 5. 创建 epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 6. 添加服务器 socket 到 epoll
    struct epoll_event event;
    event.events = EPOLLIN;  // 监听可读事件(默认水平触发)
    event.data.fd = server_fd;
    
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
    
    // 7. 事件循环
    struct epoll_event events[MAX_EVENTS];
    while (1) {
        // 阻塞等待 I/O 事件（-1 表示永久阻塞）
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            if (errno == EINTR) continue;  // 被信号中断时重试
            perror("epoll_wait failed");
            break;
        }
        
        // 8. 处理所有就绪事件
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            
            // 情况1: 服务器 socket 就绪（新连接）
            if (fd == server_fd) {
                while (1) {
                    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
                    if (new_socket == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;  // 无新连接
                        perror("accept failed");
                        break;
                    }
                    
                    // 设置新连接为非阻塞
                    if (set_nonblocking(new_socket) == -1) {
                        perror("set_nonblocking failed");
                        close(new_socket);
                        continue;
                    }
                    
                    printf("New connection: %s:%d\n", 
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    
                    // 9. 将新连接添加到 epoll
                    event.events = EPOLLIN | EPOLLET;  // 使用边缘触发模式
                    event.data.fd = new_socket;
                    
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                        perror("epoll_ctl ADD failed");
                        close(new_socket);
                    }
                }
            } 
            // 情况2: 客户端 socket 就绪（有数据可读）
            else {
                // 10. 处理客户端请求
                int total_bytes = 0;
                while (1) {
                    ssize_t bytes_read = read(fd, buffer + total_bytes, 
                                             BUFFER_SIZE - total_bytes - 1);
                    
                    if (bytes_read > 0) {
                        total_bytes += bytes_read;
                        // 继续读取直到缓冲区满或无数据（ET模式需要）
                        if (total_bytes < BUFFER_SIZE - 1) continue;
                    }
                    
                    if (bytes_read == 0) {
                        // 客户端关闭连接
                        printf("Client disconnected\n");
                        close(fd);
                        break;
                    }
                    
                    if (bytes_read == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // ET模式下读取完成
                            buffer[total_bytes] = '\0';
                            printf("Received: %s", buffer);
                            write(fd, buffer, total_bytes);  // 回显数据
                            break;
                        }
                        perror("read failed");
                        close(fd);
                        break;
                    }
                }
            }
        }
    }
    
    close(server_fd);
    close(epoll_fd);
    return 0;
}