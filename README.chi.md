# AUTO_PTR FOR C

_该文件介绍智能指针的使用方式，具体的实现方式见Test文件夹下的README.md文件_

## 介绍
*该项目为C语言实现了一种简单的，类似C++的智能指针工具*

*这使得你可以在C语言中使用智能指针！*

## 用法
### 1. 语法
```C
// 该段内容并非必须的，再处理阶段会被自动删除。但你仍可以前置该段内容以便阻止C语言编辑器报错。
#ifndef __AUTO_PTR_DUARD__
#define __AUTO_PTR_DUARD__

#define auto_ptr(__type__name__) __type__name__*
#define MALLOC(__type__name__,__size__) (__type__name__*)malloc(sizeof(__type__name__)*(__size__));

#endif

// 你可以像这样来声明一个智能指针，不要忘记初始化它。
auto_ptr(int) p = 0;

// 像这样为智能指针申请空间。 
auto_ptr(int) p2 = MALLOC(int,20)

// 之后便可以像普通指针一样使用智能指针，不同的是你不需要关心在什么时候去释放它。
p1[8] = 8;
*p2 = 6;
int x = p1[8];
int y = *p2;

// 如果你的函数返回了一个智能指针，或返回了一个包含智能指针的结构体，意味着将有更多的指针指向该内存。请提前使用ptr.cpy(size_t)来增加智能指针的引用计数。
p1.cpy(1);
return p1;
```


### 2. 示例
下面是一个使用智能指针的示例代码段:
```C
auto_ptr(int) test(){
    auto_ptr(int) ptr = MALLOC(int,20);
    ptr[8] = 10;
    ptr.cpy(1);
	return ptr;
}

int main(){
	auto_ptr(int) ptr = MALLOC(int,10);
	ptr = test();
    printf("%d",ptr[8]);
}

```


### 3. 生成目标文件
我们假设上面的源文件名为："test.c"

假设autoptr已经被添加至环境变量，你可以在源文件目录下运行该指令得到目标文件:
>./autoptr test.c target.c

得到的 target.c 文件, 可以直接通过支持GUN语法的C编译器（通常情况下指gcc）.

具体的实现方式，可以参考Test文件夹下的readme文件介绍.

## Build

你可以直接使用Release文件夹下的autoptr.exe，亦可以通过源码生成可执行文件。

just build in worksapce:
```
mkdir build
cd build
cmake ..
cmake --build .
```
release with gcc:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
```

## 注意事项

生成的文件仅支持 gcc编译

不要将普通指针与智能指针相互转化

如果你的函数返回了一个智能指针或包含智能指针的结构体，请先使用ptr.cpy(size_t) 维护引用计数

你只能使用MALLOC为智能指针申请空间，这意味着你不能使用malloc，realloc

不允许对一个已经绑定堆空间的智能指针再次赋值