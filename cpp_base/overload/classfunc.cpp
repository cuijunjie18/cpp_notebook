#include <bits/stdc++.h>
using namespace std;

class Test{
public:
    int a;
    double b;
    Test(int x,double y) : a(x),b(y) {}
    bool operator<(const Test& x){
        if (a == x.a) return b < x.b;
        return a < x.a;
    }
};

int main()
{
    Test demo1(1,10.0),demo2(10,1.0);
    if (demo1 < demo2) cout << "demo1 < demo2" << "\n";
    if (demo2 < demo1) cout << "demo2 < demo1" << "\n";
    return 0;
}