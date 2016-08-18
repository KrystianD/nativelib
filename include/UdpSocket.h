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

	bool send(const char* ip, uint16_t port, const void* data, int len) { return send(IPv4::parse(ip), port, data, len); }
	bool send(const IPv4& ip, uint16_t port, const void* data, int len);
	int recv(char ip[20], uint16_t& port, void* data, int len, uint32_t timeout = 0xffffffff)
	{
    IPv4 ipv4;
    int r = recv(ipv4, port, data, len, timeout);
    ipv4.toString(ip, 20);
    return r;
	}
	int recv(IPv4& ip, uint16_t& port, void* data, int len, uint32_t timeout = 0xffffffff);
	int recv(void* data, int len, uint32_t timeout = 0xffffffff);

	int waitForData(uint32_t timeout = 0xffffffff);

	int available();

	int getFd() const { return fd; }

private:
	int fd;

	int m_port;
};
}

#endif
