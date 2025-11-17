# c++继承

## 类继承——构造函数的显示调用

见代码
```cpp
#include <bits/stdc++.h>
using namespace std;

class Father
{
public:
    Father(int x,int y){
        a = x;
        b = y;
        std::cout << "Father" << "\n";
    }
private:
    int a,b;
};

class Son : public Father
{
public:
    Son(int x,int y) : Father(x, y){ // 需要显示调用父类的构造函数
        std::cout << "Son" << "\n";
    }
};

int main()
{
    Son demo(1,2);
    return 0;
}
```