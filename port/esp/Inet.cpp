#include "Inet.h"

#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/dns.h>

namespace nativelib {

void IPv4::toString(char* buf, int maxLen) const
{
	struct in_addr in;
	in.s_addr = _val;
	inet_ntoa_r(in, buf, maxLen);
}

const char* IPv4::str() const
{
	struct in_addr in;
	in.s_addr = _val;
	return inet_ntoa(in);
}

IPv4 IPv4::parse(const char* str)
{
	IPv4 ip;
	inet_aton(str, &ip._val);
	return ip;
}

int Inet::resolveDns(const char* addr, IPv4& ipv4)
{
	addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	addrinfo *res = 0;
	// hostent *he = gethostbyname(addr);
	// if (he == 0)
	// return -1;

	// struct in_addr** addr_list = (struct in_addr**)he->h_addr_list;

	// for (int i = 0; addr_list[i] != 0; i++) {
	// struct in_addr inaddr = *(addr_list[i]);
	// ipv4.setVal(inaddr.s_addr);
	// return 0;
	// }

	int err = lwip_getaddrinfo(addr, "80", &hints, &res);

	if (err != 0 || res == NULL) {
		if (res)
			lwip_freeaddrinfo(res);
		return -1;
	}

	sockaddr_in* addr_in = (sockaddr_in*)res->ai_addr;
	ipv4.setVal(addr_in->sin_addr.s_addr);
	return 0;
}

}
