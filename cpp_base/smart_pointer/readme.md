# c++智能指针

## 简单说明

- shared_ptr是RAII的典型实现，维护了两个指针，一个指向对象内存，一个指向控制块，控制块中有计数信息
- unique_ptr独占指针，不能赋值，只能make_unique<>
- weak_ptr是弱引用，不增加shared_ptr的计数

shared_ptr的使用示例
```cpp
std::shared_ptr<int> sp1 = std::make_shared<int>(42);
std::cout<<sp1.unique()<<std::endl; // 1
std::shared_ptr<int> sp2 = sp1;
std::shared_ptr<int> sp3(sp1);
std::shared_ptr<int> sp4(new int(44)); // Not recommended
std::cout<<sp1.use_count()<<std::endl; // 3
sp1.reset();
std::cout<<sp1.use_count()<<std::endl; // 0
std::cout<<sp2.use_count()<<std::endl; // 2
```

unique_ptr的使用示例
```cpp
std::unique_ptr<int> p1 = std::make_unique<int>(42);
std::unique_ptr<int> p2 = std::make_unique<int>(44);
int* p = p1.get();
std::cout<<*p<<std::endl; // 42
p1.swap(p2);
std::cout<<*p1<<std::endl; // 44
std::cout<<*p2<<std::endl; // 42
p1.reset();
std::cout<<p1.get()<<std::endl; // 0, first call get(), then call operator bool()
```

## 示例代码

- **[shared_ptr循环引用错误](shared_ptr_loop.cpp)**  
- **[weak_ptr解决循环引用问题](weak_ptr_loop.cpp)**    
- **[使用reset解决循环引用问题](use_reset_loop.cpp)**  

## 参考详解

知乎：https://www.zhihu.com/collection/988366857