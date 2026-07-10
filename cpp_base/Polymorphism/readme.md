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

### 解惑代码(含内存泄漏演示)

```cpp
#include <iostream>

using namespace std;

class Animal {
public:
    ~Animal() {
        std::cout << "Animal destructor" << std::endl;
    }
    virtual void move() {
        cout << "The animal is moving.\n";
    }

    virtual void makeSound() {
        cout << "The animal makes a sound.\n";
    }
};

class Cat : public Animal {
public:
    int age = 100;    
public:
    ~Cat() {
        std::cout << "Cat destructor" << std::endl;
    }
    void makeSound() {
        age = 200;
        cout << "Meow!\n";
    }
    void move() {
        age = 300;
        cout << "The cat is walking.\n";
    }
    void jump() {
        age = 400;
        cout << "The cat is jumping.\n";
    }
};

int main() {
    Animal* a = new Cat();
    cout << static_cast<Cat*>(a)->age << endl;
    a->makeSound();
    cout << static_cast<Cat*>(a)->age << endl;
    a->move();
    cout << static_cast<Cat*>(a)->age << endl;
    static_cast<Cat*>(a)->jump();
    cout << static_cast<Cat*>(a)->age << endl;
    delete a;   // 仅调用了基类的析构函数，虽然内存是释放了，但是子类的析构没有调用，可能导致内存泄漏
    return 0;
}
```
**实际上仅是实例化了一个对象，即Cat对象，而非Animal对象；且仅会调用Animal的析构函数，导致内存泄漏**

流程图如下
```txt
new Cat() 开始
    ↓
1. operator new 分配 sizeof(Cat) 内存（原始内存，未初始化）
    ↓
2. 调用 Cat 的构造函数（传入原始内存地址 this）
    ├─ 2.1 先调用 Animal 的构造函数 → 将 this 处的 vptr 指向 Animal 虚表
    ├─ 2.2 初始化 Animal 成员变量
    ├─ 2.3 初始化 Cat 成员变量（初始化列表）
    ├─ 2.4 进入 Cat 构造函数体之前 → 编译器将 this 处的 vptr 改为指向 Cat 虚表
    └─ 2.5 执行 Cat 构造函数体内的代码（此时 vptr 已经是 Cat 的了）
    ↓
3. 构造函数返回，Cat 对象完整构建完成（vptr 永远指向 Cat 表）
    ↓
4. 将构造好的对象地址赋值给 Animal* 指针 a（仅做类型转换，不改地址）
    ↓
new Cat() 结束
```

进而延伸为什么需要虚析构函数，因为析构的时候调用Animal的析构函数，发现是虚的，通过vptr去找真正的，但这时的vptr还是指向Cat的虚函数表，所以先调用了Cat的析构函数，然后再调用Animal的析构函数，这样就不会出现内存泄漏了。

正确的delete流程(假设基类析构函数是虚函数)
```txt
delete a; 开始 （a 指向堆上的 Cat 对象）
    ↓
【第一步：执行析构函数（通过虚表动态分派）】
    ├─ 1.1 通过 a 找到对象开头的 vptr，发现它指向 Cat 的虚表
    ├─ 1.2 从 Cat 虚表中取出析构函数槽位（通常是第一个虚函数或最后一个，视编译器而定）
    ├─ 1.3 跳转执行 Cat::~Cat() 的代码
    │   ├─ 1.3.1 执行 Cat 析构函数体内的代码（如 cout << "Cat dying"）
    │   └─ 1.3.2 编译器自动插入代码，按初始化逆序销毁 Cat 的成员变量
    ↓
【第二步：自动调用基类析构函数（链式回溯）】
    ├─ 2.1 Cat 析构函数执行完毕后，编译器自动调用 Animal::~Animal()
    │   ├─ 【关键动作】进入 Animal 析构函数体之前，
    │   │   编译器将对象开头的 vptr 重新赋值为指向 Animal 虚表
    │   │   （此时对象已经被降级为 Animal 视角，防止在基类析构中调用派生类函数）
    │   ├─ 2.1.1 执行 Animal 析构函数体内的代码
    │   └─ 2.1.2 按逆序销毁 Animal 的成员变量
    ↓
【第三步：释放原始内存】
    ├─ 3.1 析构链全部完成后，根据 new 时记录的内存大小（sizeof(Cat)），
    │   调用 operator delete 释放这块堆内存
    ↓
delete a; 结束
```


### 构造函数为什么不能是虚函数

详见[Virtual_construct.md](Virtual_construct.md)


## 参考文章

虚函数与多态： https://zhuanlan.zhihu.com/p/629281871  
基类指针指向子类： https://blog.csdn.net/dcrmg/article/details/83472660  