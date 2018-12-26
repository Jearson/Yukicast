#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <pthread.h>
#include "../include/station.h"

/**
 * Server representation of a connected client
 */
typedef struct client {
	struct sockaddr ip_addr;
	int cmd_port;
	int send_port;
	station_t *curr_station;

	pthread_t sender_thr;
	pthread_mutex_t client_lock;
} client_t;

client_t *client_create(struct sockaddr ip_addr, int cmd_port);
void client_destroy(client_t *client);

#endif /* CLIENT_H */
