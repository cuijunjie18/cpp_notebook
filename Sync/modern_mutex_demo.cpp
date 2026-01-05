#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int count = 0;

void threadFunction() {
    for (int i = 0; i < 100000; i++) count++;
}

void threadFunctionWithLock() {
    // guard 自动加锁和解锁(RAII)
    for (int i = 0; i < 100000; i++) {
        std::lock_guard<std::mutex> guard(mtx);
        count++;
    }

    // unique_lock (RAII)，相比guard更灵活，可以手动解锁
    for (int i = 0; i < 100000; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        lock.unlock(); // 手动解锁
    }

    // 手动加锁和解锁(非RAII)
    for (int i = 0; i < 100000; i++) {
        mtx.lock(); // 手动加锁
        count++;
        mtx.unlock(); // 手动解锁
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

    // 有锁版本 
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