#include "UdpSocket.h"

#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

namespace nativelib {
UdpSocket::UdpSocket() : fd(-1)
{
}
UdpSocket::UdpSocket(int port) : fd(-1), m_port(port)
{
}
UdpSocket::~UdpSocket()
{
	close();
}

bool UdpSocket::init()
{
	if ((fd = IMPL_SOCKET(AF_INET, SOCK_DGRAM, 0)) < 0)
		return false;

	return true;
}

bool UdpSocket::bind(int port)
{
	m_port = port;
	return bind();
}

bool UdpSocket::bind()
{
	struct sockaddr_in myaddr;

	memset((char*)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(m_port);

	if (IMPL_BIND(fd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0)
		return false;

	return true;
}

void UdpSocket::close()
{
	if (fd != -1)
		IMPL_CLOSE(fd);
	fd = -1;
}

bool UdpSocket::send(const IPv4& ip, uint16_t port, const void* data, uint32_t len)
{
	struct sockaddr_in remaddr;
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(port);
	remaddr.sin_addr.s_addr = ip.val();

	return IMPL_SENDTO(fd, (const char*)data, len, 0, (struct sockaddr*)&remaddr, sizeof(remaddr)) < 0;
}

int UdpSocket::recv(IPv4& ip, uint16_t& port, void* data, uint32_t len, uint32_t timeout)
{
	int r = waitForData(timeout);

	if (r == 1) {
		struct sockaddr_in remaddr;
		socklen_t addrlen = sizeof(remaddr);
		int recvlen = IMPL_RECVFROM(fd, data, len, 0, (struct sockaddr*)&remaddr, &addrlen);

		ip.setVal(remaddr.sin_addr.s_addr);
		port = ntohs(remaddr.sin_port);

		return recvlen;
	}

	return r;
}

int UdpSocket::recv(void* data, uint32_t len, uint32_t timeout)
{
	int r = waitForData(timeout);

	if (r == 1) {
		struct sockaddr_in remaddr;
		socklen_t addrlen = sizeof(remaddr);
		return IMPL_RECVFROM(fd, data, len, 0, (struct sockaddr*)&remaddr, &addrlen);
	}

	return r;
}

int UdpSocket::waitForData(uint32_t timeout)
{
	fd_set fds;

	struct timeval timeout_val = {0};
	struct timeval *timeout_ptr;
	if (timeout == 0xffffffff) {
		timeout_ptr = 0;
	} else {
		timeout_val.tv_sec = timeout / 1000;
		timeout_val.tv_usec = (timeout - timeout_val.tv_sec * 1000) * 1000;
		timeout_ptr = &timeout_val;
	}

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	int res = IMPL_SELECT(fd + 1, &fds, 0, 0, timeout_ptr);
	if (res == -1) {
		if (errno != EINTR) {
			return -1;
		}
	} else {
		if (FD_ISSET(fd, &fds)) {
			return 1;
		}
	}
	return 0;
}

int UdpSocket::available()
{
	int avail;
	IMPL_IOCTL(fd, FIONREAD, &avail);
	return avail;
}
}
