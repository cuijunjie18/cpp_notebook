# 编译时打桩

```shell
gcc -DCOMPILETIME -c mymalloc.c  # 先编译mymalloc.o，里面调用了实际的malloc函数
gcc -I. -o intc int.c mymalloc.o # -I.表示在当前目录搜索头文件
```