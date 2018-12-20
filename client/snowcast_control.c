#include <stdio.h>

#include "../include/client_protoc.h"
#include "../include/network.h"
#include "../include/debug.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_control <servername> <serverport> <udpport>\n");
		return -1;
	}

	// Connect to the server
	int sfd = connect_tcp(argv[1], argv[2]);
	if (sfd < 0) {
		fatal_fprintf(stderr, "Failed to establish server connection\n");
		return -1;
	}

	// bind_udp("localhost", argv[3]);
	
}