#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include <stdint.h>

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

	bool send(const char* ip, uint16_t port, const void* data, int len);
	int recv(const char ip[20], uint16_t& port, void* data, int len, uint32_t timeout = 0xffffffff);

	int getFd() const { return fd; }

private:
	int fd;

	int m_port;
};

#endif
