# 链接原理

本篇再次学习Linux上链接原理

## 原理

- 链接器(linker)本身的作用一句话：负责将编译生成的多个目标文件（.o）和库文件整合成一个可执行文件或共享库

- 编译器和汇编器生成可重定位目标文件(包括共享目标文件)。链接器生成可执行目标文件。

- 重定位是指将程序或模块中的地址引用（如变量地址、函数调用地址、跳转目标等）从链接时假设的“基地址”调整为实际加载到内存中的“运行基地址”的过程。
简单来说：让一份编译好的代码/数据，能够在内存的任意位置正确执行和访问。

## 收获

### gcc与ld

```shell
gcc -o 

# 等价指令
gcc -c # 产生.o可重定位文件
ld -o main.out <list of all .o file> # 需要链接全部需要的重定位文件，所以一般不这样编译
```

具体细节可见[对比](static_link_demo/readme.md)

### 可重定位目标文件

这里的可重定位目标文件，可以用gcc -c 生成的.o文件为例子，在Linux/Unix通常为ELF(Executable and linkable format)格式

使用下面的指令查看一个可重定位文件的格式
```shell
file <xxx.o>
```

使用下面的指令查看一个elf格式文件的符号表
```shell
readelf -s <xxx.o>

# 输出如下
Symbol table '.symtab' contains 7 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.cpp
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     4: 0000000000000000    76 FUNC    GLOBAL DEFAULT    1 main
     5: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND _Z3sumii
     6: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf

# 可见使用了main、printf、sum等函数
```

### 打桩(stub)

类别
- [编译时打桩](/compile_stub/readme.md)
- [链接时打桩](/link_stub/readme.md)
- [运行时打桩](/run_stub/readme.md)

编译时打桩需要能够访问程序的源代码，链接时打桩需要能够访问程序的可重定位对象文件，运行时打桩最灵活