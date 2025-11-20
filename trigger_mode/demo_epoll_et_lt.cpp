/*
测试代码——监控pipe的读端
预期效果：对于ET触发，只会输出一行，且阻塞；对于LT触发会输出多行,正常退出
*/

#include <bits/stdc++.h>
#include <cstdio>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
using namespace std;

enum {
    ReadEnd  = 0,
    WriteEnd = 1,
};

int pipe_fd[2];
int epoll_fd;
char buf[100005];
std::string test_str = "1234567890";

void setnoblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    if (pipe(pipe_fd) < 0){
        std::cerr << "create pipe_fd failed!" << std::endl;
        return 0;
    }

    epoll_fd = epoll_create(1);
    if (epoll_fd < 0){
        std::cout << "create epoll_fd failed!" << std::endl;
        return 0;
    }

    epoll_event test1;
    test1.events = EPOLLIN; // 默认LT
    // test1.events = EPOLLIN | EPOLLET;
    test1.data.fd = pipe_fd[ReadEnd];
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_fd[ReadEnd], &test1);

    // 测试
    (void)write(pipe_fd[WriteEnd], &test_str[0], test_str.size());

    epoll_event events[10];
    for (int j = 0; j < 5; j++) {
        int n = epoll_wait(epoll_fd, events, 10, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == pipe_fd[ReadEnd]) {
                std::cout << "ReadEnd activate!" << std::endl;
            } else if (events[i].data.fd == pipe_fd[WriteEnd]) {
                std::cout << "WriteEnd activate!" << std::endl;
            } else {
                std::cout << "Other fd activate!" << std::endl;
            }
        }
    }
    
    close(pipe_fd[ReadEnd]);
    close(pipe_fd[WriteEnd]);
    return 0;
}