#include <stdio.h>

#include "../include/client_protoc.h"
#include "../include/network.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_server <tcpport> <file1> ...\n");
		return -1;
	}

	// bind_udp("localhost", argv[3]);
	
}