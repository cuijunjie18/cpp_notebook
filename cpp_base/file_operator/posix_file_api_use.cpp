#include <bits/stdc++.h>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    // 测试写入
    int fd = open("demo.txt", O_WRONLY | O_CREAT | O_APPEND , 0644); // O_TRUNC可以覆盖文件
    if (fd == -1){
        std::cerr << "Open failed!" << std::endl;
        return 0;
    }
    std::string str = "hello,world!";
    const char *buf = str.c_str();
    if (write(fd, buf, str.size()) < 0){
        std::cerr << "Write failed!" << std::endl;
    }
    close(fd);

    // 测试读入
    int fd2 = open("demo.txt", O_RDONLY, 0644);
    if (fd2 == -1){
        std::cerr << "Open failed!" << std::endl;
        return 0;
    }
    char *buf2 = (char *)malloc(100);
    int read_n = read(fd2, buf2, 5);
    if (read_n < 0){
        std::cerr << "Read failed!" << std::endl;
    }
    assert(read_n <= 5);
    buf2[read_n] = '\0';
    std::cout << buf2 << std::endl;
    close(fd2);
    return 0;
}