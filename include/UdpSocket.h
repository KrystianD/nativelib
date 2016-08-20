#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include <stdint.h>

#include "Inet.h"

namespace nativelib {
class UdpSocket {
public:
	UdpSocket();
	UdpSocket(int port);
	~UdpSocket();

	bool init();
	bool bind();
	bool bind(int port);
	void close();

	void setPort(int port) { m_port = port; }

	bool send(const IPv4& ip, uint16_t port, const void* data, uint32_t len);
	bool send(const char* ip, uint16_t port, const void* data, uint32_t len) { return send(IPv4::parse(ip), port, data, len); }
	bool send(const InetAddress& addr, const void* data, uint32_t len) { return send(addr.ip(), addr.port(), data, len); }
	int recv(IPv4& ip, uint16_t& port, void* data, uint32_t len, uint32_t timeout = 0xffffffff);
	int recv(InetAddress& addr, void* data, uint32_t len, uint32_t timeout = 0xffffffff)
	{
		return recv(addr.ip(), addr.port(), data, len, timeout);
	}
	int recv(void* data, uint32_t len, uint32_t timeout = 0xffffffff);

	int waitForData(uint32_t timeout = 0xffffffff);

	int available();

	int getFd() const { return fd; }

private:
	int fd;

	int m_port;
};
}

#endif
