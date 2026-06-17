#include <functional>
#include <thread>
#include <iostream>
#include <unistd.h>

int result = 0;

void callbackImpl() {
    std::cout << "Hello from callback" << std::endl;
    result = 42;
}

int main() {
    std::thread t1([](std::function<void ()> callback, int a, int b) {
        std::cout << "compute result..." << std::endl;
        sleep(3);
        callback();
    }, callbackImpl, 1, 3);
    t1.join();
    std::cout << "result: " << result << std::endl;
    return 0;
}