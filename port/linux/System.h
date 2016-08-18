#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdlib.h>

namespace nativelib {
class System {
public:
	static void* malloc(int size)
	{
		return ::malloc(size);
	}
	static void free(void* ptr)
	{
		return ::free(ptr);
	}
	static uint32_t getRand()
	{
		return rand();
	}
};
}

#endif
