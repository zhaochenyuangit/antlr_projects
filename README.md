# ANTLR学习笔记
ANTLR（Another Tool For Language Recognition）是一款语言前端Parser生成器。它本身用Java实现，采用现代化的设计方式。与Flex、Bison相比，它支持更多的Parser目标语言（包括但不限于Java,C++，Python……）。同Bison一样，用户只需要手写Token符号与语法规则，输入到此工具后就生成相应的Parser，然后只要在自己的编译器中调用此Parser提供的接口就可以了，方便快捷，将写Parser的时间省下来，着重于AST→IR的部分。  

更强大的是，ANTLR可以在Parser结束后直接显示AST的gui！这比Bison那个黑箱操作实在是好太多了。

下面的介绍以ANTLR版本4.10.1为准

## ANTLR核心库安装与runtime库构建
ANTLR程序分为核心和运行时（Runtime）库两部分，核心就是用Java写的生成器本体，直接用核心程序可以生成多种目标代码的Parser。这些Parser在实际运行时必须和Runtime库链接才能正常使用。

在[这个网址](https://www.antlr.org/download.html)下载核心程序.jar文件, 然后按[此处](https://github.com/antlr/antlr4/blob/master/doc/getting-started.md) 的教程配置环境。

### 核心库部分
刚一上来就遇到了一个大坑。ANTLR 4.10.1需要Java sdk11，而Ubuntu18.04默认的jdk版本为8。虽然Ubuntu18.04也可以安装jdk11, 但懒得去折腾，直接升级虚拟机。

在Ubuntu20.04或任何一个其它支持jdk11的系统里输入以下命令安装jdk11:
```language=bash
sudo apt-get install default-jdk
```

### runtime库部分
在[这里](https://github.com/antlr/antlr4/tree/master/runtime/Cpp)下载各种目标语言的运行时库并编译，我的目标语言使用了C++（ANTLR生成的C++语言标准是C++11），因为之后是要用在LLVM框架编译器（LLVM框架语言标准是C++14）中。

> 编译C++的运行时库需要依赖包uuid-dev，可以用如下命令安装$ sudo apt-get install uuid-dev

进入目录antlr/runtime/Cpp，使用收下命令构建
```
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWITH_DEMO=True -DANTLR_JAR_LOCATION=/usr/local/lib/antlr-4.10.1-complete.jar
```
注意选择-DWITH_DEMO=True选项后会构建runtime/Cpp/demo目录下的antlr_demo文件。构建过程中需要使用antlr核心库生成Lexer与Parser，所以需要指定antlr_jar的位置（在上一步中已存放在/usr/local/lib路径下）。如果不构建demo，即默认的WITH_DEMO=false情况下，是不需要指定antlr_jar路径的。在我们自己的项目中，也可以手动地调用ANTLR核心库从.g4文件生成Lexer与Parser，再将它们的文件写到项目CMake中，但这样就不是全自动化构建了。最好要使得每一次修改.g4文件后，ANTLR自动生成的文件也跟着更新，解决办法是在CMake中将自动生成的文件作为一个custom target并依赖.g4文件。
#### 安装runtime库遇到的坑

尝试运行antlr-demo, 总是在gthread处返回__e=-1然后失败

关于什么是gthread可以看这个stackoverflow的[回答](https://stackoverflow.com/questions/62308664/why-does-gccs-threading-standard-library-implementation-throw-exceptions-if-you)

简要地说，就是antlr要调用gthread_start这个函数，而流程中一个函数需要gthread_started这个指针，当不链接pthread库（gthread是pthread的又一层封装库）时，gthread_started会是个空指针，但是这不会导致链接失败。这是一个glibc++的设计问题，不链接pthread库也不会在编译时报错，而是在运行时发现gthread_started指针为空从而判定运行时错误。

解决办法：在编译ANTLR runtime时要指定链接pthread库

```text
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
```

## 从语法文件生成Paser

```Bash
antlr4 -Dlanguage=Cpp MyGrammar.g4
```
## 如何书写语法

对于主流的语言，已有[开源仓库](https://github.com/antlr/grammars-v4)帮我们实现好了。当然，为了熟练掌握ANTLR，可以挑选一门不那么主流的语言来练手。比如[北京大学编译器课程](https://pku-minic.github.io/online-doc/)中所使用的SysY语言。
