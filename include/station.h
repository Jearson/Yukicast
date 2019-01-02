#ifndef STATION_H
#define STATION_H

#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "../include/list.h"

/**
 * Server representation of a music station
 */
#define KIB_RATE 128
#define DATA_BUFSZ KIB_RATE * 1024

typedef struct station {
	int id;
	FILE *song_file;
	char *song_name;
	uint8_t song_name_size;
	char data_buf[DATA_BUFSZ];
	list_t listeners;

	pthread_mutex_t data_lock;
	pthread_cond_t data_rdy;
} station_t;

station_t *station_create(char *filename);
int station_read_data(station_t *station);
int station_await_data(station_t *station);
void station_print(station_t *station);
void station_destroy(station_t *station);

#endif /* STATION_H */
