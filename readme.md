# lr-core-vector

用 C 语言手写一个 `std::vector`：你要做的事只有一件：根据 `include/vector.h` 的描述**把 `src/vector.c` 里的空壳函数填成能用的实现，让 `make test` 全绿**。

[vector 原理可视化](https://lingrui-studio.github.io/vector-playground/)

本题实现的是只存储 `int` 的教学版动态数组，具体约定以 [include/vector.h](include/vector.h) 为准。

## 目录结构

```
lr-core-vector/
├── readme.md         本文件
├── Makefile          构建脚本（不用改）
├── .gitignore        列举 git 需要忽视的文件
├── .clang-format     格式化要求
├── include/
│   └── vector.h      接口声明 + 函数注释（不用改，但要读懂）
├── src/
│   └── vector.c      ★ 你要实现的地方
└── tests/
    └── test.c        单元测试（不用改）
```

## 自检

补全 [include/vector.c](include/vector.c) 中的函数实现后，项目根目录运行 `make test`，若最后输出结果如下即表示你完成了本项目（本项目只有未完成和已完成两种状态，不存在中间值）：

```bash
== 通过 3393 项，失败 0 项 ==
全部通过，可以 commit & push 了
```

测试始终开启 ASan + UBSan，检测到错误会以失败状态退出，不提供关闭开关。常见错误会被直接指出来，例如：

```
ERROR: AddressSanitizer: heap-buffer-overflow on address 0x... at pc 0x...
READ of size 4 at 0x... thread T0
    #0 0x... in get src/vector.c:52
```

行号会直接指到出问题的那一行，看不懂的把完成代码和报错信息复制给 AI 问一下。

## 提交

- 完成下面的`实现思路`一节，简要说明你的各个函数是如何实现的，尤其注意内存管理的说明
- 把所有修改 commit 并 push 到 GitHub 上自己的 vector 仓库
- 在个人仓库的 Actions 页面手动触发一次自动评分工作流

## 实现思路

### 开始前关于内存管理的思考
- 首先，实现vector的是一个结构体，他由堆来管理，所以我们分配，调整，初始化，释放的，只能是里面的指针指向的东西
- 其次，指针end和cap是实现区间计算作用的，据说是因为使用这些节省内存，同时end cap只指向缓冲区内部，所以不用管他们，一赋值为NULL就无影响了
- 所以我们存放数据的地方是data，且这个data表示的地址跟vector结构体的地址不同
- 故，我们每一次动态内存分配操作应该是对于data这个指针，而end和cap是在data基础之上移动的
### 初始化
- 初始化前就要求v内没有还未释放的内存，这个靠destroy保证
- 由于指针没有初始化，所以指针不应该被使用，我们应该先让他们指向NULL
- 然后判断capacity是否合理，再移动cap和end指针
### destroy
- 只有data需要释放，只有他是动态分配的，其余只需要令其指向NULL
### size和capacity和empty
- 这三个靠指针的减法来得到大小，要注意的是，指针可能为NULL，所以此时需要判断，并将NULL的情况返回0
### get set front和back
- 这几个都是去取值的，要注意，索引值应该是size的大小减去1，同时要排除索引值有问题的情况，防止越界
### reserve
- 先写reserve，因为这个功能扩容跟pushback很相关
- 还有capacity时不扩容，等于capacity时扩容，同时为0也算相等的情况
- 要注意扩容时使用realloc，他会返回一个地址，这个得拿临时变量装，因为有可能返回的是一个NULL，需要判断
- 同时，realloc不一定在原地扩容，伴随着data的更改，end和cap也需要更改
- 同时end和cap的更改使得原来的指针可能失效了，这种情况下，size不好确认，所以应当在内部保存一遍size的值
- reserve内部移动所有指针，这样方便其他函数调用
### push_back和pop_back
- 实现size加1，需要移动指针，而由于reserve移动过指针了，所以得到的size是没有问题的，但是，由于size时刻受指针的影响，所以要注意时机去移动end指针，可能会对增加值的索引值有所影响
- 实现size减去1同理
### shrink to fit
- 如果是零，直接类似于删除，释放空间，指针都指向NULL
- 如果小于了，则需要realloc去移动，然后再移动指针，此时也需要保存原来的size值，请注意
- 如果刚好等于，不要干任何事
### clean
- 直接让end于data合并，这样size的计算就为0，假装什么都没有发生过

