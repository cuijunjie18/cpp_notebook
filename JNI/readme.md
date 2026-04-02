# c++ JNI调用

## 介绍

JNI（Java Native Interface）是 Java 与本地代码（如 C/C++）之间的桥梁，允许 Java 程序调用本地方法，也支持本地代码调用 Java 方法。

JNI 是 Java 平台的一部分，它允许：
- Java 调用 C/C++ 编写的本地方法
- C/C++ 调用 Java 方法、访问对象字段
- 实现高性能计算、硬件访问、复用已有 C/C++ 库

## 使用

**[java调用c++](javaUseCpp/readme.md)**  
**[c++调用java](cppUseJava/readme.md)**   

## 个人理解

这些跨语言的调用，如JNI调用，本质上是语言层面支持数据类型的映射，即：Java 类型 ↔ C++ 类型

## 参考

- JNI调用：https://zhuanlan.zhihu.com/p/714061732  