#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/snowcast_server_protoc.h"
#include "../include/snowcast_server.h"
#include "../include/station.h"
#include "../include/client.h"
#include "../include/list.h"
#include "../include/network.h"
#include "../include/debug.h"

pthread_t cli_thr;
pthread_t listen_thr;
int listen_sock;
int next_client_id = 0;
uint16_t num_stations = 0;
station_t *stations;

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_server <tcpport> <file1> ...\n");
		return -1;
	}

	char *tcp_port = argv[1];

	// Create stations for each file
	// TODO:
	num_stations = 0;
	// Create listening socket
	listen_sock = setup_tcp_listen(tcp_port);
	
	// Begin listening for connections
	if (pthread_create(&listen_thr, NULL, &accept_connections, NULL) != 0) {
		fatal_fprintf(stderr, "Failed to start listener for client connections\n");
		return -1;
	}
	
	// Begin the CLI thread
	if (pthread_create(&cli_thr, NULL, &do_cli, NULL) != 0) {
		fatal_fprintf(stderr, "Failed to start CLI thread\n");
		pthread_cancel(listen_thr);
		pthread_join(listen_thr, NULL);
		return -1;
	}

	pthread_join(cli_thr, NULL);
	pthread_join(listen_thr, NULL);
	fprintf(stdout, "Goodbye!\n");
	cleanup();
	return 0;
}

void *do_cli(void *arg) {
	// Begin CLI
	int run = 1;
	char buf[16];
	// If the user gives more than sizeof(buf)-1 chars, 
	// the body of the while loop executes more than once
	while (run) {
		if (fgets(buf, sizeof(buf), stdin)) {
			switch (buf[0]) {
				case '\n':
				// do nothing
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
	pthread_cancel(listen_thr);
	return NULL;
}

void *accept_connections(void *arg) {
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int cli_sock = -1, run = 1;
	client_t *client = NULL;
	while (run) {
		memset(&their_addr, 0, sizeof(their_addr));
		client = NULL;
		addr_size = sizeof(their_addr);
		info_fprintf(stderr, "Waiting for a connection...\n");
		// Accept the connections
		if ((cli_sock = accept(listen_sock, 
			(struct sockaddr*) &their_addr, &addr_size) == -1)) {
			error_fprintf(stderr, "accept_connections(): %s\n", strerror(errno));
		} else {
			char *ip = inet_ntoa(((struct sockaddr_in *) &their_addr)->sin_addr);
			info_fprintf(stderr, "Accepted connection from %s\n", ip);
			// Create client representation
			client_t *client;
			if ((client = client_create(ip, cli_sock)) == NULL) {
				error_fprintf(stderr, "Failed to create client for %s. Closing connection\n", ip);
			}
			info_fprintf(stderr, "Client IP  %s assigned ID = %d\n", client->id);
			// Begin serving the client
			if (pthread_create(&client->cmd_thr, NULL, &serve_client, client) != 0) {
				fatal_fprintf(stderr, "Failed to start thread for client <put cli addr here>\n");
			}
		}
	}
	pthread_cancel(cli_thr);
	return NULL;
}

void *serve_client(void *arg) {
	client_t *client = (client_t* ) arg;
	int cmd_sock = client->cmd_sock;
	
	// Send Welcome
	if (send_welcome(cmd_sock, num_stations) < 0) {
		error_fprintf(stderr, "Failed to send Hello\n");
		return NULL;
	}

	hello_msg_t hello;
	set_station_msg_t set_station;

	int hello_recvd = 0, first_set_station_recvd = 0, run = 1;
	uint8_t cmd_type;
	while (run) {
		// Begin listening for messages
		if (recv_cmd_type(cmd_sock, &cmd_type) < 0) {
			error_fprintf(stderr, "Failed to parse command type\n");
			continue;
		}
		switch (cmd_type) {
			case HELLO_CMD:
			memset(&hello, 0, sizeof(hello_msg_t));
			// Receive the rest of the hello
			if (recv_rest_hello(cmd_sock, &hello) != 0) {
				error_fprintf(stderr, "Failed to receive rest of Hello "
					"message. Closing connection...\n");
				run = 0;
				break;
			}

			if (hello_recvd) {
				// TODO: handle
			} else {
				// TODO: handle
			}
			break;
			case SET_STATION_CMD:
			memset(&set_station, 0, sizeof(set_station_msg_t));
			// Receive the rest of the set station command
			if (recv_rest_set_station(cmd_sock, &set_station) != 0) {
				error_fprintf(stderr, "Failed to receive rest of Set Station "
					"message. Closing connection...\n");
				run = 0;
				break;
			}
			// TODO: Determine if the set station command is for a valid station
			if (set_station.station_num >= num_stations) {
				// TODO: Send back an error message to the client
				break;
			}

			// Set the client station
			client_set_station(&stations[set_station.station_num]);

			// If this is the first time a valid station was received, 
			// begin the streamer thread
			if (!first_set_station_recvd) {
				info_fprintf(stderr, "First Set Station received\n");
				first_set_station_recvd = 1;
				if (pthread_create(&client->streamer_thr, NULL, &stream_data, client) != 0) {
					error_fprintf(stderr, "Failed to create streamer thread for "
						"client id %d, IP %s. Closing connection\n", client->id, client->ip_addr);
				}
			}
			break;
			default:
				error_fprintf(stderr, "Invalid command received: %d\n", cmd_type);
		}
	}
	client_destroy(client);
	return NULL;
}

void *stream_data(void *arg) {
	client_t *client = (client_t *) arg;
	station_t *curr_station = NULL;
	char song_data[DATA_BUFSZ];
	int bytes_read = 0;
	int run = 1;
	while (run) {
		curr_station = client_curr_station(client);
		bytes_read = station_read_data(curr_station); // Returns the number of bytes read
		if (bytes_read < 0) {
			// TODO: Handle error
		}
		// Write out data to socket
		client_send_data(client, song_data, bytes_read);
		
		// Wait until more data is fetched
		station_await_data(curr_station);
	}
	// Begin streaming data
	return NULL;
}

void print_stations() {
	info_fprintf(stderr, "print_station(): NYI\n");
}

void cleanup() {
	info_fprintf(stderr, "cleanup(): NYI\n");
}

/*
// UDP connect to the listener (connecting makes sending data faster/easier)
	info_fprintf(stderr, "Connecting to listener %s:%s\n", servname, servport);
	if ((lfd = connect_udp("localhost", udpport)) < 0) {
		fatal_fprintf(stderr, "Failed to establish listener connection\n");
		return -1;
	}
	success_fprintf(stderr, "Established UDP connection to %s:%s\n", 
				"localhost", udpport);
 */
