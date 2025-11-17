#include <bits/stdc++.h>
#include <fstream>
#include <string>

int main()
{
    // 读入文件
    std::ifstream inFile("demo.txt");
    if (!inFile.is_open()){
        std::cerr << "Failed open!" << std::endl;
        return 0;
    }
    std::string str;
    while (std::getline(inFile,str)) std::cout << str << std::endl;
    inFile.close();

    // 写入文件
    std::ofstream outFile("demo.txt");
    if (!outFile.is_open()){
        std::cerr << "Failed open!" << std::endl;
        return 0;
    }
    outFile << "New line" << std::endl;
    outFile.close();

    // 读写文件
    std::fstream ioFile("demo.txt", std::ios::in | std::ios::out | std::ios::app); // 不加 app 会覆盖原文件内容
    if (!ioFile.is_open()){
        std::cerr << "Failed open!" << std::endl;
        return 0;
    }
    ioFile << "Another new line" << std::endl;
    ioFile.seekg(0); // 定位到文件开头

    while (std::getline(ioFile,str)) std::cout << str << std::endl;
    ioFile.close();

    return 0;
}