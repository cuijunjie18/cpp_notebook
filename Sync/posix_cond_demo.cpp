#include <iostream>
#include <thread>
#include <pthread.h>

pthread_mutex_t mtx;
pthread_cond_t cv;
int count = 0;
bool ready = true;

void threadFunction() {
    for (int i = 0; i < 100000; i++) count++;
}

void threadFunctionWithLock() {
    // 使用条件变量前提：调用线程必须已锁定互斥锁
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mtx);
        while (ready == false) { // 条件不满足的时候等待
            pthread_cond_wait(&cv, &mtx);
        }
        ready = false;
        count++;
        ready = true;
        pthread_cond_signal(&cv); // 通知其他等待线程
        pthread_mutex_unlock(&mtx);
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