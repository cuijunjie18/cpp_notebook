#include <bits/stdc++.h>
using namespace std;

class B; // 前置声明

class A {
public:
    friend B; // 定义B类为A的友元类，可以访问private与protected类型的成员变量
private:
    int age{10};    
};

class B {
public:
    void PrintAClassAge(const A& instance) {
        std::cout << instance.age << std::endl;
    }    
};

int main() {
    A demoA;
    B demoB;
    demoB.PrintAClassAge(demoA);
    return 0;
}