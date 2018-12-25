#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "../include/snowcast_server.h"
#include "../include/client_protoc.h"
#include "../include/network.h"
#include "../include/debug.h"

// Want to send 128 KiB of song data per second
#define KIB_RATE 128
// Determines the buffer size of each station_t
#define BPS	KIB_RATE * 1024


pthread_t listener_thr;
int listen_sock;

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_server <tcpport> <file1> ...\n");
		return -1;
	}

	char *tcp_port = argv[1];

	// Create stations for each file
	// TODO:
	
	// Create listening socket
	// TODO:
	listen_sock = setup_tcp_listen(tcp_port);
	
	// Begin listening for connections
	pthread_create(&listener_thr, NULL, &accept_connections, NULL);
	/*
	int
     pthread_create(pthread_t *thread, const pthread_attr_t *attr,
         void *(*start_routine)(void *), void *arg);
	 */
	
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

void *accept_connections(void *args) {
	info_fprintf(stderr, "accept_connections() NYI\n");
	struct sockaddr_storage *their_addr = NULL; // We can probably just malloc this once
	socklen_t *addr_size = NULL;
	int cli_sock = -1;
	int run = 1; // This will be unset (and this thread cancelled,) when the server is told to quit on the cli
	while (run) {
		if (!their_addr) {
			their_addr = malloc(sizeof(struct sockaddr_storage));
		}
		if (!addr_size) {
			addr_size = malloc(sizeof(socklen_t));
		}

		if ((cli_sock = accept(listen_sock, 
			(struct sockaddr*) their_addr, addr_size) < 0)) {
			error_fprintf(stderr, "accept(): %s\n", strerror(errno));
		} else {
			// TODO: Create a client_t struct to keep track of the client
			// and spawn off a thread on a routine that will serve it
		}
	}

	return NULL;
}

void print_stations() {
	info_fprintf(stderr, "NYI\n");
}

void cleanup() {

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
