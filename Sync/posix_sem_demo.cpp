#include <iostream>
#include <thread>
#include <semaphore.h>

sem_t sigsem;
int count = 0;

void threadFunction() {
    for (int i = 0; i < 100000; i++) count++;
}

void threadFunctionWithLock() {
    for (int i = 0; i < 100000; i++) {
        sem_wait(&sigsem); // 等待信号量
        count++;
        sem_post(&sigsem); // 释放信号量
    }
}

int main() {
    std::thread threads[5];

    // 无锁版本
    for (int i = 0; i < 5; i++) {
        threads[i] = std::thread(threadFunction);
    }
    for (int i = 0; i < 5; i++) {
        threads[i].join();
    }
    std::cout << "Count without lock: " << count << std::endl;

    // 使用条件变量版本 
    sem_init(&sigsem, 0, 1); // 初始化信号量，初始值为1
    count = 0;
    for (int i = 0; i < 5; i++) {
        threads[i] = std::thread(threadFunctionWithLock);
    }
    for (int i = 0; i < 5; i++) {
        threads[i].join();
    }
    std::cout << "Count with lock: " << count << std::endl;
    return 0;
}