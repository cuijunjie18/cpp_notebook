/*
预期现象：阻塞时间内按下ctrl + C不会触发信号处理函数，5s后解除阻塞，原本到达的信号会被处理
*/
#include <bits/stdc++.h>
#include <chrono>
#include <csignal>
#include <unistd.h>

std::chrono::high_resolution_clock::time_point begin_timestamp,end_timestamp;

void cjj_sigint_handle(int sig){
    switch (sig){
    case SIGINT:
        std::cout << "User input ctrl + C!" << std::endl;
        break;
    case SIGTERM:
        std::cout << "User want to kill process with SIGTERM!" << std::endl;
        break;
    }
    sleep(1);
    end_timestamp = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_timestamp - begin_timestamp);
    std::cout << "use time:" << duration.count() / 1000.0 << "s" << "\n";
    exit(0); 
}

int main()
{
    begin_timestamp = std::chrono::high_resolution_clock::now();

    // 设置信号阻塞
    sigset_t prev_mask,mask;
    (void)sigemptyset(&mask);
    (void)sigaddset(&mask, SIGINT);
    (void)sigprocmask(SIG_BLOCK,&mask,&prev_mask);

    // 处理函数
    (void)signal(SIGINT,cjj_sigint_handle);

    // 5s后解除阻塞
    sleep(5);
    (void)sigprocmask(SIG_SETMASK, &prev_mask, nullptr);
    while (true);
    return 0;
}