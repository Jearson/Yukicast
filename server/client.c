#include <string.h>
#include <stdlib.h>
#include "../include/client.h"

int next_client_id = 0;

// struct sockaddr_storage
client_t *client_create(char *ip, int cmd_sock) {
	client_t *res = malloc(sizeof(client_t));
	// int connect_udp(char *dest_ip, char *port)
	// char *ip = inet_ntoa(((struct sockaddr_in *) &their_addr)->sin_addr);
	next_client_id++;
	return res;
}

station_t *client_get_curr_station(client_t *client) {
	return NULL;
}

void client_set_station(station_t *station) {

}

int client_send_data(client_t *client, void *data, int bytes) {
	return -1;
}

void client_destroy(client_t *client) {

}
