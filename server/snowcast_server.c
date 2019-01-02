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

// Main threads
pthread_t cli_thr;
pthread_t listen_thr;

// Station info
uint16_t num_stations = 0;
station_t **stations;

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_server <tcpport> <file1> ...\n");
		return -1;
	}

	char *tcp_port = argv[1];

	// Create stations for each file
	char *filename;
	station_t *station;
	for (int i=2; i < argc; i++) {
		filename = argv[i];
		if ((station = station_create(filename)) == NULL) {
			error_fprintf(stderr, 
				"Failed to create station for '%s'\n", filename);
		} 
		else {
			stations[num_stations++] = station;
		}
	}

	// Create listening socket
	int listen_sock = setup_tcp_listen(tcp_port);
	
	// Begin listening for connections
	if (pthread_create(&listen_thr, NULL, 
		&accept_connections, &listen_sock) != 0) {
		fatal_fprintf(stderr, 
			"Failed to start listener for client connections\n");
		return -1;
	}
	
	// Begin the CLI thread
	if (pthread_create(&cli_thr, NULL, &do_cli, NULL) != 0) {
		fatal_fprintf(stderr, "Failed to start CLI thread\n");
		pthread_cancel(listen_thr);
		pthread_join(listen_thr, NULL);
		return -1;
	}

	// Join threads and cleanup
	pthread_join(cli_thr, NULL);
	pthread_join(listen_thr, NULL);
	fprintf(stdout, "Cleaning up...\n");
	cleanup();
	fprintf(stdout, "Goodbye!\n");
	return 0;
}

void *do_cli(void *arg) {
	int run = 1;
	char buf[16];
	// If the user gives more than sizeof(buf)-1 chars, the body of the 
	// while loop executes more than once
	while (run) {
		if (fgets(buf, sizeof(buf), stdin)) {
			switch (buf[0]) {
				case '\n':
					break;
				case 'p':
					print_stations();
					break;
				case 'q':
					run = 0;
					break;
				default:
					fprintf(stdout, "Commands are 'p', 'q'\n");
			}
		} 
		else {
			run = 0;
		}
	}
	pthread_cancel(listen_thr);
	return NULL;
}

void *accept_connections(void *arg) {
	int listen_sock = *(int *) arg;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int cli_sock, run = 1;
	client_t *client;

	while (run) {
		// Reset
		memset(&their_addr, 0, sizeof(their_addr));
		client = NULL;
		addr_size = sizeof(their_addr);

		// Accept connections
		info_fprintf(stderr, "Waiting for a connection...\n");
		cli_sock = accept(listen_sock, 
			(struct sockaddr*) &their_addr, &addr_size);
		if (cli_sock == -1) {
			error_fprintf(stderr, "accept_connections(): %s\n", 
				strerror(errno));
		} else {
			// Create client representation
			if ((client = client_create(client->ip_addr, cli_sock)) == NULL) {
				error_fprintf(stderr, "Failed to create client for %s. "
					"Closing connection\n", client->ip_addr);
			}
			info_fprintf(stderr, "Accepted connection from %s\n", 
				client->ip_addr);
			info_fprintf(stderr, "Client IP %s assigned ID = %d\n", 
				client->id);
			// Spawn thread to begin serving the client
			if (pthread_create(&client->cmd_thr, NULL, 
				&serve_client, client) != 0) {
				fatal_fprintf(stderr, "Failed to start thread for client "
					"ID: %s, IP: %d\n", client->id, client->ip_addr);
			}
		}
	}
	pthread_cancel(cli_thr);
	return NULL;
}

void *serve_client(void *arg) {
	client_t *client = (client_t* ) arg;
	int cmd_sock = client->cmd_sock;

	hello_msg_t hello;
	set_station_msg_t set_station;

	int first_set_station = 0, run = 1;
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
			// Send back Welcome on non-gratuitous Hello
			if (client->udp_sock == -1) {
				if (send_welcome(cmd_sock, num_stations) < 0) {
					error_fprintf(stderr, "Failed to send Welcome. Closing "
						"connection...\n");
					run = 0;
					break;
				}
			}
			else {
				info_fprintf(stderr, "Gratuitous Hello. Ignoring....\n");
				char msg[] = "Gratuitous Hello Received";
				send_invalid_cmd(cmd_sock, sizeof(msg), msg);
			}
			break;

			case SET_STATION_CMD:
			memset(&set_station, 0, sizeof(set_station_msg_t));
			// Receive the rest of the set station
			if (recv_rest_set_station(cmd_sock, &set_station) != 0) {
				error_fprintf(stderr, "Failed to receive rest of Set Station "
					"message. Closing connection...\n");
				run = 0;
				break;
			}
			if (set_station.station_num >= num_stations) {
				char msg[] = "Station number out of range";
				send_invalid_cmd(cmd_sock, sizeof(msg), msg);
				break;
			}

			// Set the client station
			client_set_station(stations[set_station.station_num]);

			// If this is the first time we set a station, begin the 
			// streamer thread
			if (!first_set_station) {
				info_fprintf(stderr, "First Set Station received\n");
				first_set_station = 1;
				if (pthread_create(&client->streamer_thr, NULL, 
					&stream_data, client) != 0) {
					error_fprintf(stderr, "Failed to create streamer thread "
						"for client ID %d, IP %s. Closing connection\n", 
						client->id, client->ip_addr);
				}
			}
			break;
			default:
				error_fprintf(stderr, "Invalid command received: %d\n", 
					cmd_type);
		}
	}
	client_destroy(client);
	return NULL;
}

void *stream_data(void *arg) {
	client_t *client = (client_t *) arg;
	int cmd_sock = client->cmd_sock;
	char song_data[DATA_BUFSZ];
	station_t *station = NULL;
	int bytes_read = 0;
	int run = 1, announce = 1;
	while (run) {
		station = client_get_curr_station(client);
		// TODO: Announce if we've changed stations
		// if (prev_station_id != curr_station_id) {
		// 	announce = 1;
		// }
		// TODO: How to figure out if the file's looped?
		// - Have the station thread set station variables that each
		// streamer will read
		// TODO: Figure out station buffer details
		if (announce) {
			if (send_announce(cmd_sock, 
				station->song_name_size, station->song_name) < 0) {
				warning_fprintf(stderr, "Failed to send Announce.\n");
			}
			announce = 0;
		}
		
		// Read in the station data to local buffer
		bytes_read = station_read_data(station);
		if (bytes_read < 0) {
			// TODO: Handle error
		}
	
		// Write out data to socket
		client_send_data(client, song_data, bytes_read);

		// Wait until more data is fetched
		station_await_data(station);
	}
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
