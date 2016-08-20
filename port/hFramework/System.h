#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdlib.h>

#include <hFramework.h>

namespace nativelib {
class System {
public:
	static void* malloc(int size)
	{
		return sys.malloc(size);
	}
	static void free(void* ptr)
	{
		sys.free(ptr);
	}
	static uint32_t getRand()
	{
		return sys.getRandNr();
	}
};
}

#endif
