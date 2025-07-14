#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef __linux__
#include <sys/sysctl.h>
// sockaddr_dl and link_ntoa (not really used)
#include <net/if_dl.h>
#endif
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
// must come after net/if.h
#include <ifaddrs.h>

#ifndef SYSCTL_ERROR
#define SYSCTL_ERROR -1
#endif

#ifndef COUNTOF
#define COUNTOF(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

uint8_t const hex_map[128] = {
	['0'] = 1,
	['1'] = 2,
	['2'] = 3,
	['3'] = 4,
	['4'] = 5,
	['5'] = 6,
	['6'] = 7,
	['7'] = 8,
	['8'] = 9,
	['9'] = 10,
	['A'] = 11,
	['B'] = 12,
	['C'] = 13,
	['D'] = 14,
	['E'] = 15,
	['F'] = 16,
	['a'] = 11,
	['b'] = 12,
	['c'] = 13,
	['d'] = 14,
	['e'] = 15,
	['f'] = 16
};

// returns 0 on success
// returns -1 on malformed hex - digit outside of 0-9a-f
// returns -2 on malformed hex - hex is more than 1 byte
int mac_from_hex(uint8_t mac[6], char const * hex, size_t len, char const delim) {
	if (len > 17 || len < 11) {
		return -1;
	}

	int i = 0;
	char const * end = hex + len;
	while (i < 6 && hex != end) {
		uint16_t value = 0;
		while (hex != end && *hex != delim) {
			uint8_t digit = hex_map[*hex];
			if (!digit || digit > 16) {
				return -1;
			}
			value = value << 4 | (digit - 1);
			if (value > 255) {
				return -2;
			}
			hex++;
		}
		mac[i++] = value;
		if (hex != end) {
			hex++;
		}
	}
	return 0;
}

// returns
// 0 - success
// 1 - invalid argument
// 2 - if_nametoindex error
// 3 - sysctl get buffer size error
// 4 - sysctl returns insufficient size for a sockaddr
// 5 - malloc error
// 6 - sysctl get if_list error
// 7 - no MAC address found
int mac_from_if(uint8_t mac[6], char const * if_name) {
#ifdef __linux__
	char directory[256];
	if (!snprintf(directory, sizeof(directory), "/sys/class/net/%s/address", if_name)) {
		return 1;
	}
	FILE * file = NULL;
	if (!(file = fopen(directory, "rb"))) {
		return 1;
	}
	size_t read_size = fread(directory, 1, 256, file);
	fclose(file);

	// don't include LF in read_size
	return mac_from_hex(mac, directory, read_size - 1, ':');
#else
	size_t len = 0;
	char * buf = NULL;
	struct sockaddr_dl * s_dl = NULL;
	if (!if_name) {
		return 1;
	}

	// some BSDs have more than 6 levels. some of the 6 are unused
	int mib[6] = {
		CTL_NET,
		PF_ROUTE,
		0,
		AF_LINK,
		NET_RT_IFLIST,
		if_nametoindex(if_name)
	};

	if (0 == mib[5]) {
		perror("if_nametoindex error");
		return 2;
	}

	if (SYSCTL_ERROR == sysctl(mib, COUNTOF(mib), buf, &len, NULL, 0)) {
		perror("sysctl error getting buffer size");
		return 3;
	}

	if (len < sizeof(struct if_msghdr) + sizeof(*s_dl)) {
		perror("buffer length indicates no sockaddr in result");
		return 4;
	}

	if (!(buf = malloc(len))) {
		perror("malloc error");
		return 5;
	}

	if (SYSCTL_ERROR == sysctl(mib, COUNTOF(mib), buf, &len, NULL, 0)) {
		perror("sysctl get if_list struct error");
		return 6;
	}

	// TODO: need to validate that this struct exists
	s_dl = (struct sockaddr_dl *)(buf + sizeof(struct if_msghdr));

	// LLADDR method
	memcpy(mac, LLADDR(s_dl), 6);
	return 0;

/*	//printf("sa_family AF_LINK = %lu\n", (unsigned long)AF_LINK);
	//printf("sa_family in sockaddr: %lu\n", (unsigned long)s_dl->sdl_family);
	char const * mac_hex = link_ntoa(s_dl);
	size_t if_name_len = strlen(if_name);
	if (!strncmp(mac_hex, if_name, if_name_len)) {
		// the interfaces match
		return mac_from_hex(mac, mac_hex + if_name_len + 1, strlen(mac_hex) - if_name_len - 1, '.');
	} else {
		// find colon
		char const * mac_end = mac_hex + strlen(mac_hex);
		while (*mac_hex && *mac_hex != ':') {
			mac_hex++;
		}
		if (*mac_hex) {
			return mac_from_hex(mac, mac_hex + 1, mac_end - mac_hex - 1, '.');
		}
	}
	return 7;*/
#endif
}

// ipv4 in in_addr_t *
int comp_ifaddrs_addr(struct ifaddrs * ifa, void * ipv4) {
	in_addr_t ipv4_ = *(in_addr_t *)ipv4;

	if (AF_INET == ifa->ifa_addr->sa_family) {
		struct sockaddr_in * ifa_in = (struct sockaddr_in *)ifa->ifa_addr;
		if (ipv4_ == ifa_in->sin_addr.s_addr) {
			return 0;
		}
		return 1;
	}
	return 1;
}

int comp_ifaddrs_iface(struct ifaddrs * ifa, void * iface) {
	char const * iface_ = (char const *)iface;
	size_t iface_len = strlen(iface_);

	if (!strncmp(ifa->ifa_name, iface_, iface_len)) {
		return 0;
	}
	return 1;
}

struct ifaddrs * filter_ifaddrs(struct ifaddrs * ifa,
	int (*comp)(struct ifaddrs *, void *),
	void * data) {

	while (ifa && comp(ifa, data)) {
		ifa = ifa->ifa_next;
	}
	return ifa;
}

// returns 0 on success, else -1
int ifaddrs_ipv4_addr(struct sockaddr_in * dest, struct ifaddrs * ifa) {
	if (!ifa->ifa_addr || AF_INET != ifa->ifa_addr->sa_family) {
		return -1;
	}
	*dest = *(struct sockaddr_in *)(ifa->ifa_addr);
	return 0;
}

// return is same as snprintf
int ifaddrs_name(char * buf, size_t buf_len, struct ifaddrs * ifa) {
	return snprintf(buf, buf_len, "%s", ifa->ifa_name);
}

// returns 0 on success, else -1
int ifaddrs_ipv4_netmask(struct sockaddr_in * dest, struct ifaddrs * ifa) {
	if (!ifa->ifa_netmask || AF_INET != ifa->ifa_netmask->sa_family) {
		return -1;
	}
	*dest = *(struct sockaddr_in *)(ifa->ifa_netmask);
	return 0;
}

// returns 0 on success, else -1
// TODO: this needs to be adjusted for whether the broadcast exists or not
//	mostly this is for linux
int ifaddrs_ipv4_broadaddr(struct sockaddr_in * dest, struct ifaddrs * ifa) {
#ifdef __linux__
	if (!(ifa->ifa_flags & IFF_BROADCAST)) {
		return -1;
	}
#endif
	if (!ifa->ifa_broadaddr || AF_INET != ifa->ifa_broadaddr->sa_family) {
		return -1;
	}
	*dest = *(struct sockaddr_in *)(ifa->ifa_broadaddr);
	return 0;
}

char * sockaddr_intoa(struct sockaddr_in * src) {
	return inet_ntoa(src->sin_addr);
}

#define IPV4_HAS_ADDR 0x1
#define IPV4_HAS_NETMASK 0x2
#define IPV4_HAS_BROADCAST 0x4
struct ipv4_interface {
	struct sockaddr_in addr;
	struct sockaddr_in netmask;
	struct sockaddr_in broadcast;
	uint8_t flags;
	uint8_t mac[6];
	char if_name[256];
	uint8_t if_name_len;
};

void ipv4_interface_print(struct ipv4_interface * ipv4) {
	uint8_t * mac = &(ipv4->mac[0]);

	static char addr_buf[32];
	static char netmask_buf[32];
	static char broadcast_buf[32];
	char * addr = &addr_buf[0], * netmask = &netmask_buf[0], * broadcast = &broadcast_buf[0];
	if (ipv4->flags & IPV4_HAS_ADDR) {
		snprintf(addr, 32, "%s", sockaddr_intoa(&ipv4->addr));
	} else {
		addr = "<none>";
	}
	if (ipv4->flags & IPV4_HAS_NETMASK) {
		snprintf(netmask_buf, 32, "%s", sockaddr_intoa(&ipv4->netmask));
	} else {
		netmask = "<none>";
	}
	if (ipv4->flags & IPV4_HAS_BROADCAST) {
		snprintf(broadcast, 32, "%s", sockaddr_intoa(&ipv4->broadcast));
	} else {
		broadcast = "<none>";
	}
	printf( "  interface: %.*s\n"
			"        MAC: %02x:%02x:%02x:%02x:%02x:%02x\n"
			"       IPv4: %s\n"
			"subnet mask: %s\n"
			"  broadcast: %s\n",
			(int)ipv4->if_name_len, ipv4->if_name,
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
			addr,
			netmask,
			broadcast);
}

uint32_t subnet_mask_from_addr_broadcast(uint32_t addr, uint32_t broadcast) {
	uint32_t subnet_mask = 0;

	// broadcast = addr | ~(subnet_mask)
	// ~(subnet_mask) = broadcast ^ addr

	return subnet_mask;
}

// returns 0 on success, -1 on failure
// ipv4_interface.flags should be set to determine what is a successful call
int ipv4_interface_from_ip(struct ipv4_interface * iface, char * ip) {
	in_addr_t addr = inet_addr(ip);
	int result = 0;

	struct ifaddrs * ifaddrs = NULL;
	if (-1 == getifaddrs(&ifaddrs)) {
		fprintf(stderr, "failed to getifaddrs()\n");
		return -1;
	}

	struct ifaddrs * ifa = filter_ifaddrs(ifaddrs, comp_ifaddrs_addr, &addr);
	if (!ifa) {
		fprintf(stderr, "failed to filter_ifaddrs\n");
		goto cleanup;
	}

	int len = ifaddrs_name(&(iface->if_name[0]), sizeof(iface->if_name), ifa);
	if (len <= 0 || len > 255) {
		fprintf(stderr, "failed ifaddrs_name()\n");
		goto cleanup;
	}
	iface->if_name_len = (uint8_t)len;

	if (!ifaddrs_ipv4_addr(&(iface->addr), ifa)) {
		iface->flags |= IPV4_HAS_ADDR;
	}

	if (!ifaddrs_ipv4_netmask(&(iface->netmask), ifa)) {
		iface->flags |= IPV4_HAS_NETMASK;
	}

	if (!ifaddrs_ipv4_broadaddr(&(iface->broadcast), ifa)) {
		iface->flags |= IPV4_HAS_BROADCAST;
	}

	if((result = mac_from_if(iface->mac, iface->if_name))) {
		fprintf(stderr, "failed to get MAC address\n");
		goto cleanup;
	}

cleanup:
	freeifaddrs(ifaddrs);
	return result;
}

