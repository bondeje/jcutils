#include "psocket.h"

#ifdef _WIN32

#define MAKEWORD(low_byte, high_byte) ((uint16_t)(low_byte | (high_byte << 8)))
#define SOCKET_MODULE_VERSION MAKEWORD(2, 2)

struct socket_module VERSION = {
	.wVersion = SOCKET_MODULE_VERSION,
};

// undefine the macro to socket because we want to actually use the real WSA socket API
#undef socket
SOCKET WSAAPI socket_win32(int af, int type, int protocol) {
	SOCKET sock = INVALID_SOCKET;
	switch (af) {
		case AF_INET: {
			switch (type) {
				case SOCK_RAW: {
					sock = socket(AF_INET, SOCK_RAW, protocol);
					if (sock == INVALID_SOCKET) {
						return sock;
					}
					int hdrincl = 1;
					if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) == SOCKET_ERROR) {
						return INVALID_SOCKET;
					}
					break;
				}
				default: {
					sock = socket(af, type, protocol);
				}
			}
			break;
		}
		case AF_INET6: {
			switch (type) {
				case SOCK_RAW: {
					sock = socket(AF_INET6, SOCK_RAW, protocol);
					if (sock == INVALID_SOCKET) {
						return sock;
					}
					int hdrincl = 1;
					if (setsockopt(sock, IPPROTO_IPV6, IPV6_HDRINCL, &hdrincl, sizeof(hdrincl)) == SOCKET_ERROR) {
						return INVALID_SOCKET;
					}
					break;
				}
				default: {
					sock = socket(af, type, protocol);
				}
			}
			break;
		}
		default: {
			sock = socket(af, type, protocol);
		}
	}
	return sock;
}

ssize_t WSAAPI sendmsg(SOCKET socket, const struct msghdr * message, int flags) {
	ssize_t send_result = 0;
	if (message->msg_iovlen > 1) {
		WSABUF * buffers = malloc(message->msg_iovlen * sizeof(*buffers));
		if (!buffers) {
			// TODO: send correct error code
			return -1;
		}
		for (int i = 0; i < message->msg_iovlen; i++) {
			buffers[i].buf = (char *)message->msg_iov[i].iov_base;
			buffers[i].len = (ULONG)message->msg_iov[i].iov_len;
		}
		WSAMSG msg = {
			.name = (LPSOCKADDR)message->msg_name,
			.namelen = message->msg_namelen,
			.lpBuffers = buffers,
			.dwBufferCount = (ULONG)message->msg_iovlen,
			.dwFlags = (ULONG)message->msg_flags,
			// NOTE that control buffers are not supported
		};

		DWORD bytes_sent = 0;
		send_result = WSASendMsg(socket, &winmsg, flags, &bytes_sent, NULL, NULL);
		free(buffers);

		if (!send_result || !(out = GetLastError()) || out = WSA_IO_PENDING) {
			out = bytes_send;
		}
	} else if (message->msg_iovlen == 1) {
		WSABUF buffer = {
			.len = (ULONG)message->msg_iov[0].iov_len,
			.buf = (char *)message->msg_iov[0].iov_base,
		};
		WSAMSG msg = {
			.name = (LPSOCKADDR)message->msg_name,
			.namelen = message->msg_namelen,
			.lpBuffers = &buffer,
			.dwBufferCount = 1,
			.dwFlags = (ULONG)message->msg_flags,
			// NOTE that control buffers are not supported
		};

		DWORD bytes_sent = 0;
		send_result = WSASendMsg(socket, &winmsg, flags, &bytes_sent, NULL, NULL);
		free(buffers);

		if (!send_result || !(out = GetLastError()) || out = WSA_IO_PENDING) {
			out = bytes_send;
		}
	}
	return send_result;
}

#else

#endif
