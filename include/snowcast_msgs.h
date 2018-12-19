#ifndef SNOWCAST_MSGS_H
#define SNOWCAST_MSGS_H

#include <stdint.h>

typedef HELLO_CMD 0
typedef SET_STATION_CMD 1
typedef WELCOME_REPLY 0
typedef ANNOUNCE_REPLY 1
typedef INVALID_CMD_REPLY 2

typedef struct {
	uint8_t cmd_type;
	uint16_t udp_port;
} hello_msg_t;

typedef struct {
	uint8_t reply_type;
	uint16_t num_stations;
} welcome_msg_t;

typedef struct {
	uint8_t cmd_type;
	uint16_t udp_port;
} set_station_msg_t;

typedef struct {
	uint8_t reply_type;
	uint8_t song_name_size;
	char *song_name;
} announce_msg_t;

typedef struct {
	uint8_t reply_type;
	uint8_t reply_str_size;
	char *reply_str;
} invalid_command_msg_t;

#endif /* SNOWCAST_MSGS_H */