#include <stdio.h>

#include "../include/client_protoc.h"
#include "../include/network.h"
#include "../include/debug.h"


void print_stations() {
	info_fprintf(stderr, "NYI\n");
}

void cleanup() {

}

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_server <tcpport> <file1> ...\n");
		return -1;
	}

	// Create listening socket
	// TODO:
	
	// Begin listening for connections
	// TODO:
	
	// Begin CLI
	int run = 1;
	char buf[16];
	// If the user gives more than sizeof(buf)-1 chars, 
	// the body of the while loop executes more than once
	while (run) {
		if (fgets(buf, sizeof(buf), stdin)) {
			switch (buf[0]) {
				case 'p':
				print_stations();
				break;
				case 'q':
				run = 0;
				break;
				default:
					fprintf(stdout, "Commands are 'p', 'q'\n");
			}
		} else {
			run = 0;
		}
	}
	fprintf(stdout, "Goodbye!\n");
	cleanup();
}

/*
// UDP connect to the listener (connecting makes sending data faster)
	info_fprintf(stderr, "Connecting to listener %s:%s\n", servname, servport);
	if ((lfd = connect_udp("localhost", udpport)) < 0) {
		fatal_fprintf(stderr, "Failed to establish listener connection\n");
		return -1;
	}
	success_fprintf(stderr, "Established UDP connection to %s:%s\n", 
				"localhost", udpport);
 */
