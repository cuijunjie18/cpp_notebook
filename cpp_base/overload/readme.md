# c++重载

## 函数重载

这边函数重载较为简单，不过多阐述

## 运算符重载

运算符重载主要有两种形式，一种是成员函数形式，另一种是友元函数形式(非成员函数形式)

- 对于成员函数形式，通常仅能存在一个操作数，即重载的左操作数默认是this
  ```cpp
  class Test{
    public:
        int a;
        double b;
        Test(int x,double y) : a(x),b(y) {}
        Test operator+(const Test& x) const{
            return Test(a + x.a,b + x.b);
            // 这样会改变左操作数的属性
            // a += x.a;
            // b += x.b;
            // return *this;
        }
  };   
    
  int main()
  {
      Test demo1(1,10.0),demo2(10,1.0);
      Test demo3(0,0);
      demo3 = demo1 + demo2;
      cout << demo1.a << " " << demo1.b << "\n";
      cout << demo2.a << " " << demo2.b << "\n";
      cout << demo3.a << " " << demo3.b << "\n";
      return 0;
  }
  ```
  **注：类成员函数后面的const的作用是声明类成员函数不修改类的熟悉**

- 对于非成员函数形式，通常需要两个操作数
  ```cpp
  class Test{
  public:
      int a;
      double b;
      Test(int x,double y) : a(x),b(y) {}
      friend Test operator+(const Test& x, const Test& y);
  };
  
  Test operator+(const Test& x, const Test& y){
      return Test(x.a + y.a,x.b + y.b);
  }
  
  
  int main()
  {
      Test demo1(1,10.0),demo2(10,1.0);
      Test demo3(0,0);
      demo3 = demo1 + demo2;
      cout << demo1.a << " " << demo1.b << "\n";
      cout << demo2.a << " " << demo2.b << "\n";
      cout << demo3.a << " " << demo3.b << "\n";
      return 0;
  }
  ```

对于比较运算的使用例子，见同目录下的[classfunc.cpp](classfunc.cpp)与[friendfunc.cpp](friendfunc.cpp)

选择成员函数与非成员函数的原则
- 使用成员函数的情况
  - 运算符需要修改左操作数（如 +=, -= 等赋值运算符）
  - 一元运算符（如 ++, --, ! 等）
  - 当左操作数必须是类类型时

- 使用非成员函数的情况
  - 需要对称性（如 +, -, *, / 等）
  - 左操作数不是类类型（如 int + MyClass）
  - 流运算符（<<, >>）