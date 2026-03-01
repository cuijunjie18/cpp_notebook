# c++线程同步机制

## 互斥锁

- **[POSIX接口锁demo](posix_mutex_demo.cpp)**  
- **[现代c++锁demo](modern_mutex_demo.cpp)**  

## 条件变量

- **[POSIX接口条件变量demo](posix_cond_demo.cpp)**   
- **[现代c++条件变量demo](modern_cond_demo.cpp)**   

### 对于[POSIX接口条件变量demo](posix_cond_demo.cpp)的注意事项

对于条件变量的使用，建议先释放当前线程的互斥锁，再通知等待线程以提高性能，原因：  

当前代码在持有锁的情况下调用 pthread_cond_signal，然后立即解锁。被唤醒的线程会立即尝试获取锁，但由于锁仍被当前线程持有，它们必须阻塞在锁上，直到当前线程释放锁。这会导致额外的上下文切换和锁竞争；如果先释放锁（pthread_mutex_unlock）再发送信号（pthread_cond_signal），被唤醒的线程可以直接获得锁（如果可用），避免了不必要的阻塞，从而提升效率。

## 信号量

- **[POSIX接口信号量demo](posix_sem_demo.cpp)**   

- **c++20后才引入semaphore，故不在此使用.**
