#ifndef __INET_H__
#define __INET_H__

#include <stdint.h>

namespace nativelib {

class IPv4 {
public:
	IPv4() { }

  void setVal(uint32_t val) { _val = val; }

  void toString(char* buf, int maxLen) const;

  const char* str() const;
  uint32_t val() const { return _val; }

  static IPv4 parse(const char* str);

private:
	uint32_t _val;
};

class Inet {
public:
	static int resolveDns(const char* addr, IPv4& ipv4);

};
}

#endif
