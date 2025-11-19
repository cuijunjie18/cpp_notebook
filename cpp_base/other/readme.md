# c++语法杂项

## 结构体与类的区别

在C++中，结构体（struct）和类（class）的界限已经变得相当模糊，成员函数、构造函数、析构函数、重载等都能出现在两者中，都支持面向对象的特性，主要区别如下

```cpp
class MyClass {
    int x;        // 默认 private
public:
    int y;        // 明确 public
};

struct MyStruct {
    int x;        // 默认 public  
private:
    int y;        // 明确 private
};
```

## new 与 delete

注意： new后的内存不会自动释放，必须手动调用delete释放，否则会导致内存泄露.

```cpp
// 单个new
int *p = new int;
*p = 10;
delete p; // 释放指向的内存
p = nullptr; // 指空，防止误用

// 数组new
int *p = new int[10]; // 分配10个sizeof(int)的内存
delete[] p; // 删除方式
```

但是最好不要用new,用的话要记住：谁 new，谁 delete

更高级的c++版本可用智能指针等资源管理工具.