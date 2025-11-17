# C++多态学习

## 类别

- 静态多态： 主要通过类的成员函数重载来实现.
- 动态多态： 主要通过虚函数来实现，方式是子类继承基类.

## 收获

### 一段困惑的代码

```cpp
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructor\n";
        virtualFunction();
    }

    virtual ~Base() {
        std::cout << "Base destructor\n";
        virtualFunction();
    }

    virtual void virtualFunction() {
        std::cout << "Base virtual function\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived constructor\n";
        virtualFunction();
    }

    ~Derived() {
        std::cout << "Derived destructor\n";
    }

    void virtualFunction() override {
        std::cout << "Derived virtual function\n";
    }
};

int main() {
    Base* obj = new Derived();
    delete obj;
    return 0;
}

```

## 参考文章

虚函数与多态： https://zhuanlan.zhihu.com/p/629281871  
基类指针指向子类： https://blog.csdn.net/dcrmg/article/details/83472660  