# Java 调用 C++

- 编译，生成头文件
```shell
java -h . <java_file>
```

- 根据头文件编写c++原文件

- 编译动态库
```shell
g++ -o jniDemo.so -fpic -shared -I"<jni.h的目录>" -I"<其他头文件目录>" Demo.cpp
```
其中jni.h可以通过命令查找
```shell
find / -name "jni.h"
which java # 或者查看jdk的目录，在对应目录查看include
```
遇到报错的，找不到的头文件，直接find查找即可

- 运行
```shell
javac Demo.java && java Demo
```