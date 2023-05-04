#ifndef AUTO_PTR_H_GUARD
#define AUTO_PTR_H_GUARD

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

struct __autoptr {
	size_t cnt;
	char data[0];
};

//get the offset of the structure member in struct
#define __autoptr_offset__(_type, _name) \
	(size_t)(((_type*)0)->_name)

//get the address of the autoptr by c-ptr
#define __autoptr_container__(_type, _name, _ptr) \
	((_type*)((char*)(_ptr) - __autoptr_offset__(_type, _name)))

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

//exp: autoptr_def(int,ptr_name)
#define autoptr_def(_type, _name)\
	_type _name __attribute__((cleanup(autoptr_cleanup))) = NULL


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

#define autoptr_cpy(_name) \
	(_name != NULL && __autoptr_container__(struct __autoptr, data, _name)->cnt++) ? _name : NULL

#endif /* AUTOPTR_H */

/*--------------------------------------below is the source code----------------------------------------*/


#include<stdio.h>

int* test()
{
    autoptr_def(int*,ptr);
 autoptr_new(ptr,sizeof(int)*20);
	autoptr_new(ptr,sizeof(int)*200);
	ptr[8] = 10;
	{
		autoptr_def(char*,ptr);
 autoptr_new(ptr,sizeof(char)*20);
	}
	autoptr_cpy(ptr);
	return ptr;
}

int main()
{
	autoptr_def(int*,ptr);
 ptr = test();
    printf("%d",ptr[8]);
}