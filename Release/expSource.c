#ifndef __AUTO_PTR_DUARD__
#define __AUTO_PTR_DUARD__

#define auto_ptr(__type__name__) __type__name__*
#define MALLOC(__type__name__,__size__) (__type__name__*)malloc(sizeof(__type__name__)*(__size__))

#endif
/*--------------------------------------below is the source code----------------------------------------*/


#include<stdio.h>

auto_ptr(int) test()
{
    auto_ptr(int) ptr = MALLOC(int,20);
	ptr = MALLOC(int,200);
	ptr[8] = 10;
	{
		auto_ptr(char) ptr = MALLOC(char,20);
	}
	ptr.cpy(1);
	return ptr;
}

int main()
{
	auto_ptr(int) ptr = test();
    printf("%d",ptr[8]);
}