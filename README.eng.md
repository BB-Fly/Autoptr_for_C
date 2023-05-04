# AUTO_PTR FOR C
## Introduction
*this is a project to implement auto_ptr for C language,*

*which means you can use auto_ptr in C!*

## How to use
### 1. Grammer
```C
// this is not necessary, which will be removed later. You can write just to prevent your text-editor reporting an error. 
#ifndef __AUTO_PTR_DUARD__
#define __AUTO_PTR_DUARD__

#define auto_ptr(__type__name__) __type__name__*
#define MALLOC(__type__name__,__size__) (__type__name__*)malloc(sizeof(__type__name__)*(__size__));

#endif

// declare a auto_ptr, remember to intialize it
auto_ptr(int) p = 0;

// allocate memory for auto_ptr 
auto_ptr(int) p2 = MALLOC(int,20)

// use auto_ptr just like C_ptr, with no need to free
p1[8] = 8;
*p2 = 6;
int x = p1[8];
int y = *p2;

// if you return an auto_ptr or a struct object containing auto_ptr, please use p.cpy(size_t) first, which will increases the count by size_t.
p1.cpy(1);
return p1;
```


### 2. Exp: source file
this is an example file:
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


### 3. Get the target file
Considering the source file above is named as "test.c"

just run:
>./autoptr test.c target.c

to get target.c, which can be compiled by GNU-C (gcc) compiler.

you can compare the example files provided in test folder to find out how the auto_ptr is implemented.

## Build

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

## Notice

only useful with GNU-C

don't convert c_ptr and auto_ptr to each other

if you return an auto_ptr, or a struct with auto_ptr, remember use ptr.cpy() before

you can only use MALLOC to allocate memory for auto_ptr