#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "../include/snowcast_msgs.h"

/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(hello_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_hello(hello_msg_t *msg, char (*out)[HELLO_SZ]) {
	if (!msg || !out) {
		return -1;
	}
	uint8_t cmd_type = msg->cmd_type;
	uint16_t net_udp_port = htons(msg->udp_port);

	int off = 0;
	memcpy((*out) + off, &cmd_type, sizeof(cmd_type));
	off += sizeof(cmd_type);
	memcpy((*out) + off, &net_udp_port, sizeof(net_udp_port));

	return 0;
}

/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(welcome_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_welcome(welcome_msg_t *msg, char (*out)[WELCOME_SZ]) {
	if (!msg || !out) {
		return -1;
	}
	uint8_t reply_type = msg->reply_type;
	uint16_t net_num_stations = htons(msg->num_stations);

	int off = 0;
	memcpy((*out) + off, &reply_type, sizeof(reply_type));
	off += sizeof(reply_type);
	memcpy((*out) + off, &net_num_stations, sizeof(net_num_stations));

	return 0;
}

/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(set_station_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_set_station(set_station_msg_t *msg, char (*out)[SET_STATION_SZ]) {
	if (!msg || !out) {
		return -1;
	}
	uint8_t cmd_type = msg->cmd_type;
	uint16_t net_station_num = htons(msg->station_num);

	int off = 0;
	memcpy((*out) + off, &cmd_type, sizeof(cmd_type));
	off += sizeof(cmd_type);
	memcpy((*out) + off, &net_station_num, sizeof(net_station_num));

	return 0;
}

/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(announce_msg_t) + song_name_size 
 * bytes that will contaion the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_announce(announce_msg_t *msg, char **out) {
	if (!msg || !out) {
		return -1;
	}
	uint8_t reply_type = msg->reply_type;
	uint8_t net_song_name_size = msg->song_name_size; // single byte, so no flip
	char *net_song_name = msg->song_name;

	int off = 0;
	memcpy((*out) + off, &reply_type, sizeof(reply_type));
	off += sizeof(reply_type);
	memcpy((*out) + off, &net_song_name_size, sizeof(net_song_name_size));
	off += sizeof(net_song_name_size);
	memcpy((*out) + off, net_song_name, msg->song_name_size);
	return 0;
}

/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(invalid_cmd_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_invalid_cmd(invalid_cmd_msg_t *msg, char **out) {
	if (!msg || !out) {
		return -1;
	}
	uint8_t reply_type = INVALID_CMD_REPLY;
	uint8_t net_reply_str_size = msg->reply_str_size; // single byte, so no flip
	char *net_reply_str = msg->reply_str;

	int off = 0;
	memcpy((*out) + off, &reply_type, sizeof(reply_type));
	off += sizeof(reply_type);
	memcpy((*out) + off, &net_reply_str_size, sizeof(net_reply_str_size));
	off += sizeof(net_reply_str_size);
	memcpy((*out) + off, net_reply_str, msg->reply_str_size);
	return 0;
}

// The unmarshall commands below might not be used becuase of the variable
// length nature of some of the messages-- we can't assume that
// we would be able to receive all of the bytes for a struct without first 
// interpreting of that data whatsoever. 

/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for a hello message
 * @return       a pointer to a malloc'd hello_msg_t from the given bytes or
 * NULL on error
 */
hello_msg_t *unmarshall_hello(char (*bytes)[HELLO_SZ]) {
	if (!bytes) {
		return NULL;
	}
	hello_msg_t *res = malloc(sizeof(hello_msg_t));
	if (!res) {
		return NULL;
	}

	uint16_t host_udp_port = 0;

	int off = 0;
	memcpy(&host_udp_port, (*bytes) + off, sizeof(host_udp_port));

	res->udp_port = host_udp_port;
	return res;
}

/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for a welcome message
 * @return       a pointer to a malloc'd welcome_msg_t from the given bytes or
 * NULL on error
 */
welcome_msg_t *unmarshall_welcome(char (*bytes)[WELCOME_SZ]) {
	if (!bytes) {
		return NULL;
	}
	welcome_msg_t *res = malloc(sizeof(welcome_msg_t));
	if (!res) {
		return NULL;
	}

;	uint16_t host_num_stations = 0;

	int off = 0;
	memcpy(&host_num_stations, (*bytes) + off, sizeof(host_num_stations));
	
	res->num_stations = host_num_stations;
	return res;
}

/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for a set station message
 * @return       a pointer to a malloc'd set_station_msg_t from the given bytes or
 * NULL on error
 */
set_station_msg_t *unmarshall_set_station(char (*bytes)[SET_STATION_SZ]) {
	if (!bytes) {
		return NULL;
	}
	set_station_msg_t *res = malloc(sizeof(set_station_msg_t));
	if (!res) {
		return NULL;
	}

	uint16_t host_station_num = 0;

	int off = 0;
	memcpy(&host_station_num, (*bytes) + off, sizeof(host_station_num));
	
	res->station_num = host_station_num;
	return res;
}

/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for an announce message
 * @return       a pointer to a malloc'd announce_msg_t from the given bytes or
 * NULL on error
 */
announce_msg_t *unmarshall_announce(char (*bytes)[ANNOUNCE_SZ]) {
	if (!bytes) {
		return NULL;
	}
	announce_msg_t *res = malloc(sizeof(announce_msg_t));
	if (!res) {
		return NULL;
	}

	uint8_t host_song_name_size = 0;
	char *host_song_name = NULL;

	int off = 0;
	memcpy(&host_song_name_size, (*bytes) + off, sizeof(host_song_name_size));
	off += sizeof(host_song_name_size);

	host_song_name = malloc(host_song_name_size);
	if (!host_song_name) {
		free(res);
		return NULL;
	}
	memcpy(&host_song_name, (*bytes) + off, host_song_name_size);

	res->song_name_size = host_song_name_size;
	res->song_name = host_song_name;
	return res;
}

// TODO: Make _rest_ versions as the client/server may not no ahead of time what
// kind of message it will receive, meaning that it will need to read the type
// of the message first and from there determine how many more bytes to read
// from the socket
/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for an invalid command message
 * @return       a pointer to a malloc'd invalid_cmd_msg_t from the given bytes or
 * NULL on error
 */
invalid_cmd_msg_t *unmarshall_invalid_cmd(char (*bytes)[INVALID_CMD_SZ]) {
	if (!bytes) {
		return NULL;
	}
	invalid_cmd_msg_t *res = malloc(sizeof(invalid_cmd_msg_t));
	if (!res) {
		return NULL;
	}

	uint8_t host_reply_str_size = 0;
	char *host_reply_str = NULL;

	int off = 0;
	memcpy(&host_reply_str_size, (*bytes) + off, sizeof(host_reply_str_size));
	off += sizeof(host_reply_str_size);

	host_reply_str = malloc(host_reply_str_size);
	if (!host_reply_str) {
		free(res);
		return NULL;
	}
	memcpy(&host_reply_str, (*bytes) + off, host_reply_str_size);

	res->reply_str_size = host_reply_str_size;
	res->reply_str = host_reply_str;
	return res;
}

