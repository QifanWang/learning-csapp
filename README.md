# learning-csapp


这是我阅读深入理解计算机系统 CS:APP 并完成所有 lab 的代码，lab源代码[在此](ttp://csapp.cs.cmu.edu/3e/labs.html)。

针对这九个 lab 我记录的笔记在对应的笔记地址中，

实验名| 描述 | 笔记地址
--- | --- | ---
Data lab | 使用高度受限的c语言子集，实现简单的逻辑函数、二元补码函数和浮点函数。帮助学生理解C数据类型的位级表示以及数据操作的位级行为。| [caspp-data-lab](https://qifanwang.github.io/labs/2021/05/06/csapp-data-lab/)
Bomb lab | “二进制炸弹”是作为目标代码文件提供的程序。运行时，提示用户输入6个不同的字符串。如果其中任何一个不正确，炸弹就会“爆炸”，打印一条错误消息，并将事件记录到积分服务器上。学生必须通过分解和逆向工程程序来“拆除”他们自己的独特炸弹，以确定6个字符串应该是什么。该实验教学生理解汇编语言，并强迫他们学习如何使用调试器。 | [csapp-bomb-lab](https://qifanwang.github.io/labs/2021/05/07/csapp-bomb-lab/)
Attack lab | 为学生提供了一对具有缓冲区溢出错误的独特定制生成的 X86-64 二进制可执行文件，称为 targets。其中，一个目标容易受到代码注入攻击的影响。另一个容易受到 return-oriented 的编程攻击。要求学生通过根据代码注入或 return-oriented 的编程来挖掘漏洞以修改目标的行为。该实验向学生教授堆栈的规则，并指出编写容易受到缓冲溢出攻击的代码的危险。 | [csapp-attack-lab](https://qifanwang.github.io/labs/2021/05/11/csapp-attack-lab/)
Architecture Lab | 为学生提供一个迷你的默认 Y86-64 数组复制函数和工作管道的 Y86-64 处理器设计，该设计以数组元素的时钟周期（ cycles per array element CPE）运行这个复制函数。学生需要修改函数和处理器设计来最大程度地减少CPE。 | [csapp-arch-lab](https://qifanwang.github.io/labs/2021/05/23/csapp-arch-lab/)
Performance Lab | 学生需要优化应用核函数的性能，如卷积或矩阵转置。这个实验提供了一个清晰的缓存属性的演示，并为他们提供了低级程序优化的经验。 | [caspp-arch-lab](https://qifanwang.github.io/labs/2021/05/23/csapp-arch-lab/)
Cache Lab | 学生编写一个通用的缓存模拟器，然后优化一个小的矩阵转置核，以最大限度地减少模拟缓存的失败次数。本实验室使用Valgrind工具生成地址跟踪。 | [csapp-cache-lab](https://qifanwang.github.io/labs/2021/06/11/csapp-cache-lab/)
Shell Lab | 学生实现具有作业控制的简单 Unix shell 程序，包括 ctrl-c 和 ctrl-z 键击，fg, bg和jobs命令。使学生对Unix进程控制、信号和信号处理有一个清晰的概念。| [caspp-shell-lab](https://qifanwang.github.io/labs/2021/06/15/csapp-shell-lab/)
Malloc Lab | 实现简单的malloc、free和realloc。本实验让学生清楚地了解数据的布局和组织，并要求评估空间和时间效率之间的不同权衡。| [csapp-malloc-lab](https://qifanwang.github.io/labs/2021/10/08/csapp-malloc-lab/) 
Proxy Lab | 实现一个并发缓存Web代理。该实验室学生了解网络编程世界，并将课程中的许多概念联系在一起，例如字节序，缓存，进程控制，信号，信号处理，文件I/O，并发和同步。 | [csapp-proxy-lab](https://qifanwang.github.io/labs/2021/10/20/csapp-proxy-lab/)
