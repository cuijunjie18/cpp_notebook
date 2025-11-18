#include <bits/stdc++.h>
#include <chrono>
#include <csignal>

std::chrono::high_resolution_clock::time_point begin_timestamp,end_timestamp;

typedef int (*cjj_kkk)(int,int);

int add(int x,int y){
    return x + y;
}

void cjj_sigint_handle(int sig){
    switch (sig){
    case SIGINT:
        std::cout << "User input ctrl + C!" << std::endl;
        break;
    case SIGTERM:
        std::cout << "User want to kill process with SIGTERM!" << std::endl;
        break;
    }
    cjj_kkk func = add;
    std::cout << func(100,200) << std::endl;
    end_timestamp = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_timestamp - begin_timestamp);
    std::cout << "use time:" << duration.count() / 1000.0 << "s" << "\n";
    exit(0); 
}

int main()
{
    (void)signal(SIGINT,cjj_sigint_handle);
    (void)signal(SIGTERM,cjj_sigint_handle);
    begin_timestamp = std::chrono::high_resolution_clock::now();
    while (true);
    return 0;
}