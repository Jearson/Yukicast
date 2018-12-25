#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../../include/snowcast_control_protoc.h"
#include "../../include/snowcast_control.h"
#include "../../include/network.h"
#include "../../include/debug.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_control "
			"<servername> <serverport> <udpport>\n");
		return -1;
	}

	char *servname = argv[1], *servport = argv[2], *udpport = argv[3];
	int sfd;
	uint16_t num_stations;

	// TCP connect to the server
	info_fprintf(stderr, "Connecting to server %s:%s\n", servname, servport);
	if ((sfd = connect_tcp(servname, servport)) < 0) {
		fatal_fprintf(stderr, "Failed to establish server connection\n");
		return -1;
	}
	success_fprintf(stderr, "Established TCP connection to %s:%s\n", 
				servname, servport);

	int udp_port = strtol(udpport, NULL, 10);
	// Send a hello
	send_hello(udp_port);
	// Receive a Welcome
	if (recv_welcome(&num_stations) < 0) {
		fatal_fprintf(stderr, "Failed to receive Welcome message.");
		return -1;
	}
	fprintf(stdout, "There are %d stations.\n", num_stations);

	// Begin the CLI
	int run = 1;
	char buf[16];
	int station_num = -1;
	// If the user gives more than sizeof(buf)-1 chars, 
	// the body of the while loop executes more than once
	while (run) {
		if (fgets(buf, sizeof(buf), stdin)) {
			switch (buf[0]) {
				case 'q':
				run = 0;
				break;
				default:
					// Try to interpret the string as a number
					station_num = strtol(buf, NULL, 10);
					if (station_num == 0 && errno != 0) {
						error_fprintf(stderr, "%s", strerror(num_stations));
						errno = 0;
					} else {
						// Send Set Station message
						send_set_station(station_num);
					}
			}
			station_num = -1;
		} else {
			run = 0;
		}
	}
	fprintf(stdout, "Goodbye!\n");
	
}
