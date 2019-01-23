#include "TcpSocket.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <stdarg.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

namespace nativelib
{

	TcpSocket::TcpSocket()
	{
		sockFd = -1;
	}
	TcpSocket::~TcpSocket()
	{
	}

	TcpSocket::Result TcpSocket::connect(const std::string& address, uint16_t port, uint32_t timeout)
	{
		hostent* he = gethostbyname(address.c_str());
		if (he == NULL) {
			// LOG("unable to resolv %s", address);
			return ResultResolveError;
		}
		struct in_addr** addr_list = (struct in_addr**)he->h_addr_list;

		struct sockaddr_in server;
		for (int i = 0; addr_list[i] != NULL; i++) {
			server.sin_addr = *(addr_list[i]);
			// LOG("%s resolved to %s", address, inet_ntoa(server.sin_addr));
		}

		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		sockFd = socket(AF_INET, SOCK_STREAM, 0);

		fcntl(sockFd, F_SETFL, O_NONBLOCK);

		// LOG("connecting via fd %d to %s:%d", sockFd, address, port);
		if (::connect(sockFd, (struct sockaddr*)&server, sizeof(server)) < 0) {
			if (errno != EINPROGRESS) {
				close();
				// LOG_PERROR("connect failed. Error");
				return TcpSocket::ResultConnectError;
			}
		}

		// connect timeout
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(sockFd, &fdset);
		struct timeval timeout_val = {0, 0};
		struct timeval* timeout_ptr;
		if (timeout == 0xffffffff) {
			timeout_ptr = 0;
		} else {
			timeout_val.tv_sec = timeout / 1000;
			timeout_val.tv_usec = (timeout - timeout_val.tv_sec * 1000) * 1000;
			timeout_ptr = &timeout_val;
		}

		if (select(sockFd + 1, NULL, &fdset, NULL, timeout_ptr) == 1) {
			int so_error;
			socklen_t len = sizeof(so_error);
			getsockopt(sockFd, SOL_SOCKET, SO_ERROR, &so_error, &len);

			fcntl(sockFd, F_SETFL, ~O_NONBLOCK);

			if (so_error == 0) {
				// LOG("connected");
				return TcpSocket::ResultOK;
			} else {
				close();
				// LOG("connection rejected");
				return TcpSocket::ResultConnectionRejectedError;
			}
		} else {
			close();
			// LOG("connection timeout");
			return TcpSocket::ResultConnectionTimeoutError;
		}
	}
	TcpSocket::Result TcpSocket::bind(int port)
	{
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(port);

		sockFd = socket(AF_INET, SOCK_STREAM, 0);

		int enable = 1;
		setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

		// fcntl(sockFd, F_SETFL, O_NONBLOCK);

		// LOG("connecting via fd %d to %s:%d", sockFd, address, port);
		if (::bind(sockFd, (struct sockaddr*)&server, sizeof(server)) < 0) {
			printf("er\r\n");
			if (errno != EINPROGRESS) {
				close();
				// LOG_PERROR("connect failed. Error");
				return TcpSocket::ResultError;
			}
		}
		return TcpSocket::ResultOK;
	}
	TcpSocket::Result TcpSocket::listen()
	{
		::listen(sockFd, 10);
		return ResultOK;
	}
	TcpSocket::Result TcpSocket::accept(TcpSocket& socket)
	{
		sockaddr_in clientaddr;
		socklen_t addrlen = sizeof(clientaddr);

		if ((socket.sockFd = ::accept(sockFd, (sockaddr * ) & clientaddr, &addrlen)) < 0) {
			perror("accept");
			return ResultError;
		}

		return ResultOK;
	}
	void TcpSocket::close()
	{
		::close(sockFd);
		sockFd = -1;
	}

	int TcpSocket::read(void* data, uint32_t offset, uint32_t length, uint32_t timeout)
	{
		fd_set fdset;
		struct timeval timeout_val = {0};
		struct timeval* timeout_ptr;
		if (timeout == 0xffffffff) {
			timeout_ptr = 0;
		} else {
			timeout_val.tv_sec = timeout / 1000;
			timeout_val.tv_usec = (timeout - timeout_val.tv_sec * 1000) * 1000;
			timeout_ptr = &timeout_val;
		}

		int tmpSockFd = sockFd;

		if (tmpSockFd == -1) {
			// LOG_WARN("using closed socket (read)");
			return -1;
		}

		FD_ZERO(&fdset);
		FD_SET(tmpSockFd, &fdset);
		int r = select(tmpSockFd + 1, &fdset, NULL, NULL, timeout_ptr);
		if (r == 0)
			return 0;
		if (r == -1) {
			if (errno != EINTR) {
				// string error = getErrnoString();
				// LOG_ERROR("select error (%d, %s)", res, error.c_str());
				return -1;
			} else {
				return 0;
			}
		}

		int res = ::read(tmpSockFd, ((uint8_t*)data) + offset, length);
		if (res == -1 && errno == EAGAIN)
			return 0;
		if (res <= 0) {
			// LOG_PERROR("read failed");
			printf("read err %d\r\n", res);
			return -1;
		}
		return res;
	}
	int TcpSocket::write(const void* data, uint32_t offset, uint32_t length, uint32_t timeout)
	{
		int tmpSockFd = sockFd;

		if (tmpSockFd == -1) {
			// LOG_WARN("using closed socket (write)");
			return -1;
		}
		int res = ::send(tmpSockFd, ((uint8_t*)data) + offset, length, 0);
		if (res < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// LOG_WARN("write EAGAIN");
				return -2;
			}
			// LOG_PERROR("send failed");
			return -1;
		}
		return res;
	}

	bool TcpSocket::addToEPoll(crosslib::EPoll& epoll)
	{
		return epoll.addRead(sockFd);
	}

	bool TcpSocket::removeFromEPoll(crosslib::EPoll& epoll)
	{
		return epoll.remove(sockFd);
	}

	int TcpSocket::available()
	{
		int bytes_available;
		ioctl(sockFd, FIONREAD, &bytes_available);
		return bytes_available;
	}

	int TcpSocket::waitForData(uint32_t timeout)
	{
		fd_set fds;

		struct timeval timeout_val = {0};
		struct timeval* timeout_ptr;
		if (timeout == 0xffffffff) {
			timeout_ptr = 0;
		} else {
			timeout_val.tv_sec = timeout / 1000;
			timeout_val.tv_usec = (timeout - timeout_val.tv_sec * 1000) * 1000;
			timeout_ptr = &timeout_val;
		}

		FD_ZERO(&fds);
		FD_SET(sockFd, &fds);

		int res = select(sockFd + 1, &fds, 0, 0, timeout_ptr);
		if (res == -1) {
			if (errno != EINTR) {
				return -1;
			}
		} else {
			if (FD_ISSET(sockFd, &fds)) {
				return 1;
			}
		}
		return 0;
	}

}
