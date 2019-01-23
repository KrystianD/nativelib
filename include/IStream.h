#ifndef __ISTREAM_H__
#define __ISTREAM_H__

#include <cstddef>
#include <cstdint>

#include <crosslib.h>

class IStream
{
public:
	IStream() {}
	virtual ~IStream() {}

	virtual int read(void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff) = 0;
	virtual int write(const void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff) = 0;

	virtual int readAll(void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff)
	{
		uint8_t* _data = (uint8_t*)data + offset;
		int pos = 0;
		uint64_t startTime = crosslib::OS::getTimeMS();
		while (length) {
			uint32_t toWait;
			if (timeout == 0xffffffff) {
				toWait = 0xffffffff;
			} else {
				uint32_t elapsed = crosslib::OS::getTimeMS() - startTime;
				if (elapsed >= timeout)
					break;
				toWait = timeout - elapsed;
			}

			int ret = read(_data, 0, length, toWait);
			if (ret < 0)
				return ret;
			_data += ret;
			pos += ret;
			length -= ret;
		}
		return pos;
	}

	virtual int writeAll(const void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff)
	{
		uint8_t* _data = (uint8_t*)data + offset;
		int pos = 0;
		uint64_t startTime = crosslib::OS::getTimeMS();
		while (length) {
			uint32_t toWait;
			if (timeout == 0xffffffff) {
				toWait = 0xffffffff;
			} else {
				uint32_t elapsed = crosslib::OS::getTimeMS() - startTime;
				if (elapsed >= timeout)
					break;
				toWait = timeout - elapsed;
			}

			int ret = write(_data, 0, length, toWait);
			if (ret < 0)
				return ret;
			_data += ret;
			pos += ret;
			length -= ret;
		}
		return pos;
	}
};

class NullStream : public IStream
{
public:
	int read(void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff) override
	{
		crosslib::OS::sleep(timeout);
		return 0;
	}
	int write(const void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff) override { return length; }
	int readAll(void* data, uint32_t offset, uint32_t length, uint32_t timeout) override { return read(data, offset, length, timeout); }
};

#endif
