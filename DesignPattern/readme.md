# c++设计模式

## 单例模式

c++单例模式作用：单例模式确保一个类只有一个实例，并提供全局访问点.

最推荐的实现方式——使用静态局部变量
```cpp
#include <iostream>

class Singleton
{
public:
    ~Singleton(){
        std::cout<<"destructor called!"<<std::endl;
    }
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
    static Singleton& get_instance(){
        static Singleton instance;
        return instance;

    }
private:
    // 保证构造函数存在
    Singleton(){
        std::cout<<"constructor called!"<<std::endl;
    }
};

int main(int argc, char *argv[])
{
    Singleton& instance_1 = Singleton::get_instance();
    Singleton& instance_2 = Singleton::get_instance();
    return 0;
}
```

其中
```cpp
Singleton(const Singleton&)=delete;
Singleton& operator=(const Singleton&)=delete;
```
是c++11引入的显示删除，删除了拷贝构造与拷贝赋值运算，因为对于单例模式没有意义.

**[单例demo](single_demo.cpp)**

## 参考

c++单例：https://www.cnblogs.com/sunchaothu/p/10389842.html