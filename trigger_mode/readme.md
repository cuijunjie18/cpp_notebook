# epoll的事件触发模式

## 类别

- LT(Level Trigger)，即水平触发，**一个事件只要有，就会一直触发.**
- ET(Edge Trigger)，即边沿触发，**只有一个事件从无到有才会触发.**

<br>

## 注意事项

- ET触发模式下，对应监测的文件描述符fd必须设置为非阻塞，因为对于ET触发的fd，需要用循环把数据读完，如果是阻塞模式，则会在最后一次read中阻塞，导致程序超时.

## 参考

csdn： https://blog.csdn.net/cloud323/article/details/130153126