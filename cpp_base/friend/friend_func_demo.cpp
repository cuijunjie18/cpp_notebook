#include <bits/stdc++.h>
using namespace std;

class A {
public:
    A() = default;
    ~A() = default;
private:
    int age{10};
    friend void PrintAgeWithFriend(const A& instance);
};

// 友元函数(类的非成员函数)，可以访问类的私有成员变量
void PrintAgeWithFriend(const A& instance) {
    std::cout << instance.age << std::endl;
}

// 普通函数，不可访问类的私有成员变量，编译报错
void PrintAgeWithoutFriend(const A& instance) {
    std::cout << instance.age << std::endl;
}

int main() {
    A demo;
    PrintAgeWithFriend(demo);
    PrintAgeWithoutFriend(demo);
    return 0;
}