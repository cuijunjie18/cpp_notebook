# gcc与ld

## 直接-o

```shell
gcc -o main_direct.out main.cpp sum.cpp
```

## -c再link

```shell
gcc -c main.cpp sum.cpp
ld \
  --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed \
  -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
  -pie -z now -z relro \
  -o main.out \
  /usr/lib/x86_64-linux-gnu/Scrt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  /usr/lib/gcc/x86_64-linux-gnu/11/crtbeginS.o \
  -L/usr/lib/gcc/x86_64-linux-gnu/11 \
  -L/usr/lib/x86_64-linux-gnu \
  -L/lib/x86_64-linux-gnu \
  -L/usr/lib \
  /tmp/ccqhTGSC.o /tmp/cctqkZUU.o \
  -lgcc --push-state --as-needed -lgcc_s --pop-state \
  -lc \
  -lgcc --push-state --as-needed -lgcc_s --pop-state \
  /usr/lib/gcc/x86_64-linux-gnu/11/crtendS.o \
  /usr/lib/x86_64-linux-gnu/crtn.o
```