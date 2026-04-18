# 运行时打桩

编译
```shell
gcc -DRUNTIME -shared -fpic -o mymalloc.so mymalloc.c -ldl
gcc -o intr int.c
```

执行
```shell
LD_PRELOAD="./mymalloc.so" ./intr
```

现代执行报错，因为现代的libc.so库中printf会调用malloc，导致无限递归