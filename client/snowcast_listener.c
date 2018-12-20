#include <stdio.h>

#include "../include/network.h"
#include "../include/debug.h"

int main(int argc, char **argv) {
	if (argc < 1) {
		fprintf(stderr, "./snowcast_listener <udpport>\n");
		return -1;
	}

	char *udpport = argv[1];

	int lfd;
	if ((lfd = setup_udp_socket("localhost", udpport)) < 0) {
		fatal_fprintf(stderr, "Failed to open local socket\n");
		return -1;
	}
	
	// Read from the socket and write to stdout
	
}