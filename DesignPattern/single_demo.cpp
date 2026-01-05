#include <bits/stdc++.h>

class Test {
public:
    static Test& getInstance() {
        static Test instance;
        return instance;
    }
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;
    Test() = default;
public:
    int x = 0;
};

int main() {
    Test& t1 = Test::getInstance();
    Test& t2 = Test::getInstance();
    std::cout << &t1 << std::endl;
    std::cout << &t2 << std::endl;
    t1.x = 42;
    std::cout << t2.x << std::endl; // Should print 42
    return 0;
}