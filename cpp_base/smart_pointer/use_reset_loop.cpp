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
    
    a->b_ptr = b;  // b的引用计数变为2（main中的b + a->b_ptr）
    b->a_ptr = a;  // a的引用计数变为2（main中的a + b->a_ptr）
    a->b_ptr.reset();  // 手动重置a->b_ptr，b的引用计数减1变为1
    
    // 离开作用域时（使用reset()手动打破循环引用）：
    // 1. main中的a释放，a引用计数减1变为1（还剩1，来自b->a_ptr）
    // 2. main中的b释放，b引用计数减1变为0，B对象被销毁
    // 3. B对象销毁时，b->a_ptr被清理，a引用计数减1变为0，A对象被销毁
    // 结果：通过手动reset()成功打破了循环引用，两个对象都能正常销毁！
    return 0;
}
