#include <iostream>
#include <bits/stdc++.h>
#include <list>
using namespace std;

class Demo {
public:
    int a;
    int& get() {
        return a;
    }
};

int main() {
    Demo x;
    x.a = 10;
    int a1 = x.get();
    int& a2 = x.get();
    std::cout << &a1 << " " << &a2 << " " << &x.a << std::endl;
    return 0;
}