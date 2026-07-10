# C++ STL容器

## 基本对比

如果从内存布局和底层数据结构来看，C++ 标准库中的所有容器底层实现可以分为四大类：

| 底层数据结构 | 对应的 STL 容器 | 内存特点 |
| :--- | :--- | :--- |
| **动态数组** | `std::vector`、`std::array`（固定大小）、`std::string` | 连续内存块。支持 O(1) 的随机访问，但在尾部之外的地方插入/删除开销很大（需移动元素）。 |
| **双向/单向链表** | `std::list`（双向链表）、`std::forward_list`（单向链表） | 非连续内存，由节点组成。插入/删除非常快（O(1)），但**不支持**随机访问（`[]` 操作），内存占用较大（需存储指针）。 |
| **双端队列（分段连续）** | `std::deque` | 由一段一段的连续小块内存组成，维护一个中央索引表。支持 O(1) 随机访问，且在**头尾两端**的插入/删除效率极高。 |
| **红黑树（平衡二叉树）** | `std::set`、`std::map`、`multiset`、`multimap` | 自动排序，插入/查找/删除的时间复杂度为 O(log n)。 |
| **哈希表（散列表）** | `std::unordered_set`、`unordered_map` 等 | 无序存储，插入/查找/删除的平均时间复杂度为 O(1)，最坏退化为 O(n)。 |

## vector实现原理

简单原理：https://c.biancheng.net/view/6901.html  
从零实现：
- https://zhuanlan.zhihu.com/p/1891152193391924575  
- https://blog.csdn.net/yuuki233233/article/details/156194483  

## 适用于学习的简易版STL实现参考

[MyTinySTL](git@github.com:Alinshans/MyTinySTL.git)