#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h> 

#include "../include/network.h"
#include "../include/debug.h"

#define BUFSZ 1024

int main(int argc, char **argv) {
	if (argc < 1) {
		fprintf(stderr, "./snowcast_listener <udpport>\n");
		return -1;
	}

	char *udpport = argv[1];

	int lfd;
	if ((lfd = bind_udp(udpport)) < 0) {
		fatal_fprintf(stderr, "Failed to open local socket\n");
		return -1;
	}
	
	// Read from the socket and write to stdout
	char buf[BUFSZ];
	int bytes_read;
	while (1) {
		bytes_read = read(lfd, buf, BUFSZ);
		if (write(STDOUT_FILENO, buf, bytes_read) < 0) {
			fatal_fprintf(stderr, "Write failed: %s\n", strerror(errno));
			return -1;
		}
	}
	if (bytes_read < 0) {
		fatal_fprintf(stderr, "Write failed: %s\n", strerror(errno));
		return -1;
	}
	
}
