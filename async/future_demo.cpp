// 编译指令
// g++ -g -o future_demo future_demo.cpp -std=c++11 

// async调用函数直接返回，等价于promise 通过 set_value 设置值关联future

#include <future>
#include <unistd.h>
#include <iostream>
#include <thread>

// 计算函数只负责业务逻辑，返回普通类型
int compute() {
    std::cout << "Computing result..." << std::endl;
    sleep(3); // 模拟耗时操作
    return 42;
}

// 工作线程函数：接收 std::promise 的引用
void compute_with_promise(std::promise<int>& prom) {
    std::cout << "Computing result..." << std::endl;
    sleep(3); // 模拟耗时操作
    int result = 42;
    
    // 核心操作：将结果设置到 promise 中
    // 注意：set_value 只能被调用一次。如果多次调用，会抛出 std::future_error 异常。
    prom.set_value(result);
}

void demo1() {
    std::cout << "========================== demo 1 ==========================" << std::endl;
    // 使用 std::async 启动异步任务，返回 std::future<int>
    std::future<int> result_future = std::async(std::launch::async, compute);
    
    // 在等待结果期间，主线程可以继续做其他事情
    std::cout << "Main thread is doing other work..." << std::endl;
    
    // 调用 .get() 获取结果。如果计算未完成，这里会阻塞等待
    int result = result_future.get();
    std::cout << "Final result: " << result << std::endl;
}

void demo2() {
    std::cout << "========================== demo 2 ==========================" << std::endl;
    std::promise<int> prom;
    std::future<int> result_future = prom.get_future();
    std::thread t(compute_with_promise, std::ref(prom));

    // 在等待结果期间，主线程可以继续做其他事情
    std::cout << "Main thread is doing other work..." << std::endl;

    int result = result_future.get();
    std::cout << "Final result: " << result << std::endl;
    t.join();
}

int main() {
    demo1();
    demo2();
    return 0;
}