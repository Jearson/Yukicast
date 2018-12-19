#ifndef SERVER_PROTOC_H
#define SERVER_PROTOC_H

#include "snowcast_msgs.h"

// Number of miliseconds to wait before timing out
#define RECV_TIMEOUT 100

int recv_cmd_type(uint8_t *cmd_type);
int recv_hello(uint16_t *udp_port);
int recv_set_station(uint16_t *station_num);
int send_welcome(uint16_t num_stations);
int send_announce(uint8_t song_name_size, char *song_name);
int send_invalid_cmd(uint8_t reply_str_size, char *reply_str);

#endif /* SERVER_PROTOC_H */