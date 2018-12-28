#ifndef SNOWCAST_MSGS_H
#define SNOWCAST_MSGS_H

#include <stdint.h>

/**
 * Constants and structures used by the Snowcast protocol.
 */

// Client message numbers
#define HELLO_CMD 0
#define SET_STATION_CMD 1

// Server Message numbers
#define WELCOME_REPLY 0
#define ANNOUNCE_REPLY 1
#define INVALID_CMD_REPLY 2

// Message Structs
typedef struct  __attribute__ ((__packed__)) {
	uint8_t cmd_type;
	uint16_t udp_port;
} hello_msg_t;

typedef struct  __attribute__ ((__packed__)) {
	uint8_t reply_type;
	uint16_t num_stations;
} welcome_msg_t;

typedef struct  __attribute__ ((__packed__)) {
	uint8_t cmd_type;
	uint16_t station_num;
} set_station_msg_t;

typedef struct  __attribute__ ((__packed__)) {
	uint8_t reply_type;
	uint8_t song_name_size;
	char *song_name;
} announce_msg_t;

typedef struct  __attribute__ ((__packed__)) {
	uint8_t reply_type;
	uint8_t reply_str_size;
	char *reply_str;
} invalid_cmd_msg_t;

// Used for sending messages
#define HELLO_SZ sizeof(hello_msg_t)
#define WELCOME_SZ sizeof(welcome_msg_t)
#define SET_STATION_SZ sizeof(set_station_msg_t)
#define ANNOUNCE_SZ sizeof(announce_msg_t)
#define INVALID_CMD_SZ sizeof(invalid_cmd_msg_t)
// Used for receiving messages
// Messages are first received by their type, then by the rest of the message
#define REST_HELLO_SZ HELLO_SZ - sizeof(uint8_t)
#define REST_WELCOME_SZ WELCOME_SZ - sizeof(uint8_t)
#define REST_SET_STATION_SZ SET_STATION_SZ - sizeof(uint8_t)
#define REST_ANNOUNCE_SZ ANNOUNCE_SZ - sizeof(uint8_t)
#define REST_INVALID_CMD_SZ INVALID_CMD_SZ - sizeof(uint8_t)

/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(hello_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_hello(hello_msg_t *msg, char (*out)[HELLO_SZ]);
/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(welcome_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_welcome(welcome_msg_t *msg, char (*out)[WELCOME_SZ]);
/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(set_station_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_set_station(set_station_msg_t *msg, char (*out)[SET_STATION_SZ]);
/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(announce_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_announce(announce_msg_t *msg, char (*out)[ANNOUNCE_SZ]);
/**
 * Converts the message into its network byte representation
 * @param  msg the struct to marshal
 * @param  out a buffer of sizeof(invalid_cmd_msg_t) bytes that will contaion
 * the network byte representation
 * @return     0 on success, -1 on failure
 */
int marshall_invalid_cmd(invalid_cmd_msg_t *msg, char (*out)[INVALID_CMD_SZ]);
/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for a hello message
 * @return       a pointer to a malloc'd hello_msg_t from the given bytes or
 * NULL on error
 */
hello_msg_t *unmarshall_hello(char (*bytes)[HELLO_SZ]);
/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for a welcome message
 * @return       a pointer to a malloc'd welcome_msg_t from the given bytes or
 * NULL on error
 */
welcome_msg_t *unmarshall_welcome(char (*bytes)[WELCOME_SZ]);
/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for a set station message
 * @return       a pointer to a malloc'd set_station_msg_t from the given bytes or
 * NULL on error
 */
set_station_msg_t *unmarshall_set_station(char (*bytes)[SET_STATION_SZ]);
/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for an announce message
 * @return       a pointer to a malloc'd announce_msg_t from the given bytes or
 * NULL on error
 */
announce_msg_t *unmarshall_announce(char (*bytes)[ANNOUNCE_SZ]);
/**
 * Marshalls network bytes into its struct representation
 * @param  bytes pointer to a buffer containing the network-order bytes 
 * for an invalid command message
 * @return       a pointer to a malloc'd invalid_cmd_msg_t from the given bytes or
 * NULL on error
 */
invalid_cmd_msg_t *unmarshall_invalid_cmd(char (*bytes)[INVALID_CMD_SZ]);

#endif /* SNOWCAST_MSGS_H */
