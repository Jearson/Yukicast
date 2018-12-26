#ifndef STATION_H
#define STATION_H

#include <pthread.h>
#include <stdio.h>
#include "../include/list.h"

/**
 * Server representation of a music station
 */
#define KIB_RATE 128
#define DATA_BUFSZ KIB_RATE * 1024

typedef struct station {
	FILE *song_file;
	char *song_name;
	int song_name_size;
	char data_buf[DATA_BUFSZ];
	list_t listeners;

	pthread_mutex_t data_lock;
	pthread_cond_t data_rdy;
} station_t;

station_t *station_create();
void station_print(station_t *s);
void station_destroy(station_t *s);

#endif /* STATION_H */
