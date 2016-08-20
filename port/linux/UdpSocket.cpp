#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define IMPL_SOCKET   ::socket
#define IMPL_BIND     ::bind
#define IMPL_SENDTO   ::sendto
#define IMPL_RECVFROM ::recvfrom
#define IMPL_SELECT   ::select
#define IMPL_IOCTL    ::ioctl
#define IMPL_CLOSE    ::close

#include "../../tpl/UdpSocket.cpp"
