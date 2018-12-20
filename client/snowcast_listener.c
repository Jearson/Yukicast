#include <stdio.h>

#include "../include/network.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_listener <udpport>\n");
		return -1;
	}

	// bind_udp("localhost", argv[3]);
	
}