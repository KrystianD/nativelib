#ifndef __INET_H__
#define __INET_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace nativelib
{
	class IPv4
	{
	public:
		IPv4() {}

		void setVal(uint32_t val) { _val = val; }

		void toString(char* buf, int maxLen) const;

		const char* str() const;
		uint32_t val() const { return _val; }

		void toBytes(void* data) const
		{
			memcpy(data, &_val, 4);
		}

		static IPv4 parse(const char* str);
		static IPv4 fromBytes(const void* data)
		{
			IPv4 ip;
			memcpy(&ip._val, data, 4);
			return ip;
		}

	private:
		uint32_t _val;
	};

	class InetAddress
	{
		IPv4 _ip;
		uint16_t _port;

	public:
		InetAddress() {}
		InetAddress(const IPv4& ip, uint16_t port) : _ip(ip), _port(port) {}
		InetAddress(const char* ip, uint16_t port) : _ip(IPv4::parse(ip)), _port(port) {}

		IPv4& rip() { return _ip; }
		uint16_t& rport() { return _port; }

		const IPv4& ip() const { return _ip; }
		uint16_t port() const { return _port; }

		void setIP(const IPv4& ip) { _ip = ip; }
		void setIP(const char* ip) { _ip = IPv4::parse(ip); }
		void setIPFromBytes(const void* data) { _ip = IPv4::fromBytes(data); }
		void setPort(uint16_t port) { _port = port; }

		const char* str() const
		{
			static char d[22];
			sprintf(d, "%s:%d", _ip.str(), _port);
			return d;
		}
	};

	class Inet
	{
	public:
		static int resolveDns(const char* addr, IPv4& ipv4);
	};
}

#endif
