#include <stdint.h>
#include <stdio.h>

#include "net.c"

int main(int narg, char ** args) {
	if (narg < 2) {
		printf("Usage: %s <ip address>\n", args[0]);
		return 1;
	}

	struct ipv4_interface ipv4 = {
		.flags = IPV4_HAS_ADDR | IPV4_HAS_BROADCAST
	};
	int result = ipv4_interface_from_ip(&ipv4, args[1]);
	ipv4_interface_print(&ipv4);
	return result;
}
