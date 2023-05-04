#ifndef CSTR_H_GUARD
#define CSTR_H_GUARD


#include<string>

using namespace std;

namespace AUTO_PTR_STR
{
static constexpr char* NoFind = "NOT FIND";
static constexpr char* Header =
"#ifndef AUTO_PTR_H_GUARD\n\
#define AUTO_PTR_H_GUARD\n\
\n\
#include <stdlib.h>\n\
#include <stdint.h>\n\
#include <stddef.h>\n\
\n\
struct __autoptr {\n\
	size_t cnt;\n\
	char data[0];\n\
};\n\
\n\
//get the offset of the structure member in struct\n\
#define __autoptr_offset__(_type, _name) \\\n\
	(size_t)(((_type*)0)->_name)\n\
\n\
//get the address of the autoptr by c-ptr\n\
#define __autoptr_container__(_type, _name, _ptr) \\\n\
	((_type*)((char*)(_ptr) - __autoptr_offset__(_type, _name)))\n\
\n\
//cnt -= 1, if cnt equals to 0, clean up the autoptr\n\
__attribute__((always_inline))\n\
static inline void autoptr_cleanup(void *_ptr)\n\
{\n\
	void *ptr = (void *)*(void **)_ptr;\n\
	if (ptr == NULL)\n\
		return;\n\
	struct __autoptr *container =\n\
	    __autoptr_container__(struct __autoptr, data, ptr);\n\
	if (--container->cnt == 0)\n\
		free(container);\n\
}\n\
\n\
//exp: autoptr_def(int,ptr_name)\n\
#define autoptr_def(_type, _name)\\\n\
	_type _name __attribute__((cleanup(autoptr_cleanup))) = NULL\n\
\n\
\n\
#define autoptr_new(_name, _size) \\\n\
do {\\\n\
	struct __autoptr *container = NULL;\\\n\
	if (_name != NULL){\\\n\
		container = __autoptr_container__(struct __autoptr, data, ptr);\\\n\
		free(container);\\\n\
	}\\\n\
	container = (struct __autoptr*)malloc (sizeof(struct __autoptr) + _size);\\\n\
	container->cnt = 1;\\\n\
	_name = (__typeof__ (_name)) container->data;\\\n\
} while(0)\n\
\n\
#define autoptr_cpy(_name) \\\n\
	(_name != NULL && __autoptr_container__(struct __autoptr, data, _name)->cnt++) ? _name : NULL\n\
\n\
#endif /* AUTOPTR_H */\n";
}


#endif