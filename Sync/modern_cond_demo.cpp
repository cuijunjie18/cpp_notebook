#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
int count = 0;
bool ready = true;

void threadFunction() {
    for (int i = 0; i < 100000; i++) count++;
}

void threadFunctionWithLock() {
    // 使用条件变量前提：调用线程必须已锁定互斥锁
    for (int i = 0; i < 100000; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return ready; }); // 等待条件变量：ready为true时被唤醒，否则等待
        ready = false; // 设置为false，模拟某种条件变化
        count++;
        ready = true;
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