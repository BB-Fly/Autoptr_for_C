# 智能指针是如何实现的？
*该文件将详细介绍智能指针的实现逻辑.*

## auto_ptr with C
>每个智能指针对象都会指向一个名为 __autoptr 的结构体, 该结构体在堆空间中的位置，位于申请的内存的前方。 
```C
struct __autoptr {
	size_t cnt;
	char data[0]; // C_ptr points to this
};
```
>使用下面两个函数，可以方便的根据智能指针得到结构体的位置；反之亦然。
```c
//get the offset of the structure member in struct
#define __autoptr_offset__(_type, _name) \
	(size_t)(((_type*)0)->_name)

//get the address of the autoptr by c-ptr
#define __autoptr_container__(_type, _name, _ptr) \
	((_type*)((char*)(_ptr) - __autoptr_offset__(_type, _name)))

```
>当智能指针离开了它的作用域，便会调用该函数。当引用计数为0时，内存被释放。
```c
//cnt -= 1, if cnt equals to 0, clean up the autoptr
__attribute__((always_inline))
static inline void autoptr_cleanup(void *_ptr)
{
	void *ptr = (void *)*(void **)_ptr;
	if (ptr == NULL)
		return;
	struct __autoptr *container =
	    __autoptr_container__(struct __autoptr, data, ptr);
	if (--container->cnt == 0)
		free(container);
}
```

>使用以下方式声明一个智能指针。由于使用attribute绑定了cleanup，对象消亡时会自动调用析构函数。
```C
#define autoptr_def(_type, _name)\
	_type _name __attribute__((cleanup(autoptr_cleanup))) = NULL
```
>使用下面的方式为智能指针申请空间。
```c
#define autoptr_new(_name, _size) \
do {\
	struct __autoptr *container = NULL;\
	if (_name != NULL){\
		container = __autoptr_container__(struct __autoptr, data, ptr);\
		free(container);\
	}\
	container = (struct __autoptr*)malloc (sizeof(struct __autoptr) + _size);\
	container->cnt = 1;\
	_name = (__typeof__ (_name)) container->data;\
} while(0)
```

>复制智能指针时，增加引用计数
```c
#define autoptr_cpy(_name) \
	(_name != NULL && __autoptr_container__(struct __autoptr, data, _name)->cnt++) ? _name : NULL

```

有了上述实现，便可以用这种方式来安全的使用指针：
```c
void* test(){
	autoptr_def(void*, ptr);
	autoptr_new(ptr, sizeof(int) * 20);
	autoptr_new(ptr, sizeof(int) * 200);
    ptr[8]=10;
	return autoptr_cpy(ptr);
}

int main(){
	autoptr_def(void*, ptr);
	ptr = test();
    printf("%d",ptr[8]);
}
```
但上面的方式并不方便，因此我实现了一个文本转换器，能够将下面的文件转换为上面的格式:
```c
auto_ptr(int) test(){
    auto_ptr(int) ptr = MALLOC(int,20);
    ptr[8] = 10;
    ptr.cpy(1);
	return ptr;
}

int main(){
	auto_ptr(int) ptr = test();
    printf("%d",ptr[8]);
}
```

文本转换的具体的方式不再赘述，详细内容可参考C++源码.