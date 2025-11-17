#include <bits/stdc++.h>
using namespace std;

class Test{
public:
    int a;
    double b;
    Test(int x,double y) : a(x),b(y) {}
    friend bool operator<(const Test& x,const Test& y);
};

bool operator<(const Test& x,const Test& y){
    if (x.a == y.a) return x.b < y.b;
    return x.a < y.a;
}

int main()
{
    Test demo1(1,10.0),demo2(10,1.0);
    if (demo1 < demo2) cout << "demo1 < demo2" << "\n";
    if (demo2 < demo1) cout << "demo2 < demo1" << "\n";
}