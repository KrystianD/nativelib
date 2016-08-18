#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>

#define IMPL_SOCKET   ::lwip_socket
#define IMPL_BIND     ::lwip_bind
#define IMPL_SENDTO   ::lwip_sendto
#define IMPL_RECVFROM ::lwip_recvfrom
#define IMPL_SELECT   ::lwip_select
#define IMPL_IOCTL    ::lwip_ioctl

#include "../../tpl/UdpSocket.cpp"
