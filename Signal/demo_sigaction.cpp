#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void sigaction_handler(int sig) {
    printf("收到信号: %d\n", sig);
}

void sigaction_handler2(int sig, siginfo_t *info, void *context) {
    printf("收到信号: %d\n", sig);
    printf("发送者进程ID: %d\n", info->si_pid);
    printf("用户ID: %d\n", info->si_uid);
}

int main() {
    struct sigaction sa;
    
    // 初始化结构体
    memset(&sa, 0, sizeof(sa));
    
    // 使用 sa_sigaction 或者 sa_handler
    sa.sa_sigaction = sigaction_handler2;
    // sa.sa_handler = sigaction_handler;
    
    // 使用 SA_SIGINFO 标志获取更多信息
    sa.sa_flags = SA_SIGINFO;
    
    // 设置信号处理
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    // （可选）在进行信号处理函数的时候，阻塞所有信号
    sigfillset(&sa.sa_mask);  
    
    printf("进程ID: %d\n", getpid());
    printf("发送测试信号: kill -SIGUSR1 %d\n", getpid());
    
    while(1) {
        sleep(1);
    }
    
    return 0;
}