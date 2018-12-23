#include <stdio.h>

#include "../include/client_protoc.h"
#include "../include/network.h"
#include "../include/debug.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_control "
			"<servername> <serverport> <udpport>\n");
		return -1;
	}

	char *servname = argv[1], *servport = argv[2], *udpport = argv[3];
	int sfd;

	// TCP connect to the server
	info_fprintf(stderr, "Connecting to server %s:%s\n", servname, servport);
	if ((sfd = connect_tcp(servname, servport)) < 0) {
		fatal_fprintf(stderr, "Failed to establish server connection\n");
		return -1;
	}
	success_fprintf(stderr, "Established TCP connection to %s:%s\n", 
				servname, servport);


	// Begin the CLI
	
	
}
