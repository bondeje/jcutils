#ifndef PSOCKET_H
#define PSOCKET_H

/*
POSIX API additions for robustness:

1) Use "SOCKET" type for socket instead of "int"
2) Use "SOCKET_ERROR" for checking socket functions for errors instead of < 0 or == -1
3) Use "INVALID_SOCKET" for checking socket() failures instead of < 0 or == -1
4) Call "init_socket_module()" once at the beginning of "main"
*/

#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>
#include <stdint.h>
#include "ptypes.h"
#include "puio.h"
#include "netinet/pin.h"

typedef WSADATA socket_module;
static inline int init_socket_module(uint16_t version, socket_module * mod) {
	return WSAStartup(version, mod);
}

typedef int_least32_t socklen_t;
typedef SSIZE_T ssize_t;

struct msghdr {
	void * msg_name;
	socklen_t msg_na me_len;
	struct iovec *msg_iov;
	int msg_iovlen;
	void * msg_control;
	socklen_t msg_controllen;
	int msg_flags;
};

#define recvfrom(socket, pmessage, length, flags, src_addr, src_len) \
	recvfrom(socket, (char *)(pmessage), (int)(length), flags, src_addr, src_len) \
#define sendto(socket, pmessage, length, flags, dest_addr, dest_len) \
	sendto(socket, (const char *)(pmessage), (int)(length), flags, dest_addr, dest_len)
#define setsockopt(socket, level, option, pvalue, length) \
	setsockopt(socket, level , option, (const char *)(pvalue), length)

#define socket(family, type, protocol) \
	socket_win32(family, type, protocol)

SOCKET WSAAPI socket_win32(int af, int type, int protocol);

ssize_t WSAAPI sendmsg(SOCKET socket, const struct msghdr * message, int flags);

#else

#include <sys/socket.h>

typedef int SOCKET;
#define SOCKET_ERROR -1

#define MAKEWORD(low_byte, high_byte) ((uint16_t)(low_byte | high_byte << 8))

typedef struct socket_module {
	uint16_t wVersion;
} socket_module;

extern struct socket_module VERSION;

static inline int init_sockets(uint16_t version, struct socket_module * mod) {
	*mod = VERSION;
	return 0;
}

#endif

#endif
