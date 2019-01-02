#include "../include/station.h"

station_t *station_create(char *filename) {
	/*
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
	 */
	return NULL;
}
int station_read_data(station_t *station) {
	return -1;
}
int station_await_data(station_t *station) {
	return -1;
}
void station_print(station_t *station) {

}
void station_destroy(station_t *station) {

}
