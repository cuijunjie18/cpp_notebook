#include <bits/stdc++.h>
#include <pthread.h>

pthread_mutex_t lock;
int count = 0;

void* threadFunction(void* arg) {
    for (int i = 0; i < 100000; i++) count++;
    return nullptr;
}

void* threadFunctionWithLock(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        count++;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

int main() {
    pthread_t threads[5];

    // 无线程同步
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], nullptr, threadFunction, nullptr);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], nullptr);
    }
    std::cout << "Final count: " << count << std::endl;

    // 引入线程同步机制
    count = 0;
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], nullptr, threadFunctionWithLock, nullptr);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], nullptr);
    }
    std::cout << "Final count with lock: " << count << std::endl;
    return 0;
}