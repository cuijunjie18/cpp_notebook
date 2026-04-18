# 链接时打桩

```shell
gcc -DLINKTIME -c mymalloc.c
gcc -c int.c
gcc -Wl,--wrap,malloc -Wl,--wrap,free -o intl int.o mymalloc.o
```

Linux 静态链接器支持用 --wrap f 标志进行链接时打桩。这个标志告诉链接器，把对符号 f 的引用解析成 wrap_f（前缀是两个下划线），还要把对符号 __real_f（前缀是两个下划线）的引用解析为 f