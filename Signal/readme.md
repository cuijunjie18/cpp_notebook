# Linux信号处理

<br>

## 信号接收机制的理解

接收信号。

1.内核为每个进程在pending位向量维护着待处理信号的集合，而在blocked位向量中维护着被阻塞的信号集合。所以在任何时刻一种类型的信号只会被接收一次，在处理它的时候，会先把该类型的信号block。进程可以忽略信号，也可以捕获这个信号，执行信号处理程序。

2.当内核从一个异常处理程序返回（进程调度也属于一种异常？定时器中断？），准备把控制传递给某个进程p时，它会检查进程p的未被阻塞的待处理信号集合（pending & ~blocked）。如果这个集合不为空，那么内核选择集合中的某个信号k（通常是编号最小的信号，所以Linux信号编号还是特意的呢，编号越小，优先级越高），并进入k的处理程序。

Ps:在block的时候，来的信号会不会标记到pending里？

答案：会的。执行信号的处理动作称为信号递达（Delivery），信号从产生到递达之间的状态，称为信号未决（Pending）。进程可以选择阻塞（Block）某个信号。被阻塞的信号在产生时将保持在未决状态，直到进程解除对此信号的阻塞，才执行递达的动作。注意，阻塞和忽略是不同的，只要信号被阻塞就不会递达，而忽略是在递达之后可选的一种处理动作。忽略也可以说是信号处理程序的一种。

<br>

## 设置信号处理函数

- 核心函数
  ```cpp
  /* Set the handler for the signal SIG to HANDLER, returning the old
   handler, or SIG_ERR on error.
   By default `signal' has the BSD semantic.  */
  extern __sighandler_t signal (int __sig, __sighandler_t __handler)
  ```

- demo示例
[信号处理函数demo](demo_signal.cpp)  
[多信号处理函数demo](demo_signal_multi.cpp)  
[sigaction处理函数demo](demo_sigaction.cpp)  

<br>

## 信号阻塞

- 核心函数集
	```cpp
	#include <signal.h>

	/* 改变blocked向量的值，若oldset!=null，会用来保存以前blocked向量的值 */
	int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); // how可以选SIG_BLOCK、SIG_UNBLOCK、SIG_SETMASK

	/* 初始化set为空集 */
	int sigemptyset(sigset_t *set);

	/* 初始化set全为1，每个信号都填入blocked向量 */
	int sigfillset(sigset *set);

	/* 添加、删除signum到set */
	int sigaddset(sigset_t *set, int signum);
	int sigdelset(sigset_t *set, int signum);

	/* set中对应signum是否置1 */
	int sigismember(const sigset_t *set, int signum);
	```

- demo示例
[阻塞信号与解除demo](demo_signal_block_unblock.cpp)

<br>

## 参考

Linux异常控制流：https://blog.csdn.net/WMLWONDER/article/details/53728630