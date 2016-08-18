#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdlib.h>

extern "C" {
#include <FreeRTOS.h>
}

namespace nativelib {
class System {
public:
	static void* malloc(int size)
	{
		return pvPortMalloc(size);
	}
	static void free(void* ptr)
	{
		vPortFree(ptr);
	}
	static uint32_t getRand()
	{
		return rand();
	}
};
}

#endif
