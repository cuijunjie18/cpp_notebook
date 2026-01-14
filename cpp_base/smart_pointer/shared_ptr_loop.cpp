#include <bits/stdc++.h>

class B; // 前置声明

class A {
public:
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

class B {
public:
    std::shared_ptr<A> a_ptr;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    
    a->b_ptr = b;  // b的引用计数变为2
    b->a_ptr = a;  // a的引用计数变为2
    
    // 离开作用域时：
    // 1. main中的b释放，b引用计数减1（还剩1，来自a->b_ptr）
    // 2. main中的a释放，a引用计数减1（还剩1，来自b->a_ptr）
    // 结果：两者引用计数都不为0，都不会被销毁！
    return 0;
}