#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

namespace nativelib
{
	class System
	{
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
			uint32_t v;
			randomBytes(&v, 0, 4);
			return v;
		}
		static uint32_t randomBytes(void* data, uint32_t offset, uint32_t length)
		{
			FILE* f = fopen("/dev/urandom", "rb");
			int r = fread((uint8_t*)data + offset, 1, length, f);
			fclose(f);
			return r;
		}
	};
}

#endif
