# How auto_ptr implemented
*this file will introduce how auto_ptr is implemented.*
## auto_ptr with C
>an auto_ptr points to a struct named __autoptr, which is followed by the data memory allocated 
```C
struct __autoptr {
	size_t cnt;
	char data[0]; // C_ptr points to this
};
```
>with these two function, we can get auto_ptr with C_ptr or vice versa
```c
//get the offset of the structure member in struct
#define __autoptr_offset__(_type, _name) \
	(size_t)(((_type*)0)->_name)

//get the address of the autoptr by c-ptr
#define __autoptr_container__(_type, _name, _ptr) \
	((_type*)((char*)(_ptr) - __autoptr_offset__(_type, _name)))

```
>when an auto_ptr leaves it's scope, it will call this function
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

>to use a auto_ptr, we need to declare first
```C
#define autoptr_def(_type, _name)\
	_type _name __attribute__((cleanup(autoptr_cleanup))) = NULL
```
>then we can malloc some memory for it
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

>when copy an auto_ptr, use this function
```c
#define autoptr_cpy(_name) \
	(_name != NULL && __autoptr_container__(struct __autoptr, data, _name)->cnt++) ? _name : NULL

```

this is an example target file:
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
However, above is inconvenient, and I want use auto_ptr like this:
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

So I implement a text conversion program to achieve this.