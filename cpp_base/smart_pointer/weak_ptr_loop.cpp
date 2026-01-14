#include <bits/stdc++.h>

class B; // 前置声明

class A {
public:
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

class B {
public:
    std::weak_ptr<A> a_ptr;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    
    a->b_ptr = b;  // b的引用计数变为2
    b->a_ptr = a;  // 使用weak_ptr，不会增加a的引用计数，a的引用计数仍为1
    
    // 离开作用域时（已使用weak_ptr解决循环引用）：
    // 1. main中的a释放，a引用计数减1变为0，A对象被销毁
    // 2. A对象销毁时，a->b_ptr被清理，b引用计数减1变为1
    // 3. main中的b释放，b引用计数减1变为0，B对象被销毁
    // 结果：使用weak_ptr成功避免了循环引用，两个对象都能正常销毁！
    return 0;
}
