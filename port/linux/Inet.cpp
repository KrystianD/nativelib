#include "Inet.h"

#include <arpa/inet.h>
#include <netdb.h>

namespace nativelib {

void IPv4::toString(char* buf, int maxLen) const
{
  inet_ntop(AF_INET, &_val, buf, maxLen);
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
	inet_pton(AF_INET, str, &ip._val);
  return ip;
}

int Inet::resolveDns(const char* addr, IPv4& ipv4)
{
	hostent* he = gethostbyname(addr);
	if (he == 0)
		return -1;

	struct in_addr** addr_list = (struct in_addr**)he->h_addr_list;

	for (int i = 0; addr_list[i] != 0; i++) {
		struct in_addr inaddr = *(addr_list[i]);
		ipv4.setVal(inaddr.s_addr);
		return 0;
	}

	return -1;
}

}
