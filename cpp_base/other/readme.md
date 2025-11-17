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