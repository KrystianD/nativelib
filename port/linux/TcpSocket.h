#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <stdint.h>
#include <string>

#include <IStream.h>
#include <crosslib.h>

namespace nativelib
{
	class TcpSocket : public IStream
	{
	public:
		enum Result
		{
			ResultOK, ResultError, ResultResolveError, ResultConnectError, ResultConnectionRejectedError, ResultConnectionTimeoutError
		};

		TcpSocket();
		~TcpSocket() override;

		int getFd() const { return sockFd; }

		Result connect(const std::string& address, uint16_t port, uint32_t timeout = 0xffffffff);
		Result bind(int port);
		Result listen();
		Result accept(TcpSocket& socket);
		void close();

		int read(void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff) override;
		int write(const void* data, uint32_t offset, uint32_t length, uint32_t timeout = 0xffffffff) override;

		bool addToEPoll(crosslib::EPoll& epoll);
		bool removeFromEPoll(crosslib::EPoll& epoll);

		int available();
		int waitForData(uint32_t timeout);

	private:
		int sockFd;
	};
}

#endif
