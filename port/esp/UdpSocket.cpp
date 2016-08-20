#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>

#define IMPL_SOCKET   ::lwip_socket
#define IMPL_BIND     ::lwip_bind
#define IMPL_SENDTO   ::lwip_sendto
#define IMPL_RECVFROM ::lwip_recvfrom
#define IMPL_SELECT   ::lwip_select
#define IMPL_IOCTL    ::lwip_ioctl
#define IMPL_CLOSE    ::lwip_close

#if LWIP_SO_RCVBUF != 1
#error LWIP_SO_RCVF must be equal to 1
#endif

#include "../../tpl/UdpSocket.cpp"
