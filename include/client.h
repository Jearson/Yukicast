#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <pthread.h>
#include "../include/station.h"

/**
 * Server representation of a connected client
 */
typedef struct client {
	int id;
	char *ip_addr;
	int cmd_sock;
	int udp_sock;

	pthread_t cmd_thr;
	pthread_t streamer_thr;
	pthread_mutex_t client_lock;

	// Synchronized data
	station_t *curr_station;
} client_t;

client_t *client_create(char *ip, int cmd_sock);
station_t *client_get_curr_station(client_t *client);
void client_set_station(station_t *station);
int client_send_data(client_t *client, void *data, int bytes);
void client_destroy(client_t *client);

#endif /* CLIENT_H */
