#include <string.h>
#include <stdlib.h>
#include "../include/client.h"

extern int next_client_id;

client_t *client_create(char *ip, int cmd_sock) {
	client_t *res = malloc(sizeof(client_t));
	
	return res;
}

station_t *client_curr_station(client_t *client) {
	return NULL;
}

void client_set_station(station_t *station) {

}

int client_send_data(client_t *client, void *data, int bytes) {
	return -1;
}

void client_destroy(client_t *client) {

}
