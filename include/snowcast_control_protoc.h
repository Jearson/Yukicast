#ifndef CLIENT_PROTOC_H
#define CLIENT_PROTOC_H

#include "snowcast_msgs.h"

// Number of miliseconds to wait before timing out
#define RECV_TIMEOUT 100

/**
 * Sends a hello message to the server
 * @param  udp_port the udp port to send song data to
 * @return          	// TODO: Figure out how to report error types (enums?)
 */
int send_hello(uint16_t udp_port);

/**
 * Sends a set station message to the server
 * @param  station_num the station number the client wants to listen to
 * @return         		// TODO: Figure out how to report error types (enums?)
 */
int send_set_station(uint16_t station_num);

/**
 * Receives the reply number of a message sent by the server
 * @param  reply_type A pointer to a uint8_t that will be filled with a 
 * reply number: 0 for Welcome, 1 for Announce, and 2 for Invalid Command
 * @return             // TODO: Figure out how to report error types (enums?)
 */
int recv_reply_type(uint8_t *reply_type);

/**
 * Receives the rest of a Welcome message from the server.
 * @param  num_stations Pointer to uint16_t that will be filled with the number
 * of stations the server supports.
 * @return              // TODO: Figure out how to report error types (enums?)
 */
int recv_rest_welcome(uint16_t *num_stations);

/**
 * Receives the rest of an Announce message from the server
 * @param  song_name_size Pointer to uint8_t that will be filled with the number
 * of bytes to receive for the song name
 * @param  song_name      Pointer to a buffer that will be filled with a zstring
 * of the song name. The buffer is allocated during the call and should be 
 * freed after use.
 * @return                // TODO: Figure out how to report error types (enums?)
 */
int recv_rest_announce(uint8_t *song_name_size, char **song_name);

/**
 * Receives the rest of an Invalid Command message from the server
 * @param  reply_str_size Pointer to uint8_t that will be filled with the number
 * of bytes to received for the reply string.
 * @param  reply_str      Pointer to a buffer that will be filled with a zstring
 * of the reply string. The buffer is allocated dduring the call and should be
 * freed after use.
 * @return                // TODO: Figure out how to report error types (enums?)
 */
int recv_rest_invalid_cmd(uint8_t *reply_str_size, char **reply_str);

/**
 * Receives a Welcome message from the server.
 * @param  num_stations Pointer to uint16_t that will be filled with the number
 * of stations the server supports.
 * @return              // TODO: Figure out how to report error types (enums?)
 */
int recv_welcome(uint16_t *num_stations);

/**
 * Receives an Announce message from the server
 * @param  song_name_size Pointer to uint8_t that will be filled with the number
 * of bytes to receive for the song name
 * @param  song_name      Pointer to a buffer that will be filled with a zstring
 * of the song name. The buffer is allocated during the call and should be 
 * freed after use.
 * @return                // TODO: Figure out how to report error types (enums?)
 */
int recv_announce(uint8_t *song_name_size, char **song_name);

/**
 * Receives an Invalid Command message from the server
 * @param  reply_str_size Pointer to uint8_t that will be filled with the number
 * of bytes to received for the reply string.
 * @param  reply_str      Pointer to a buffer that will be filled with a zstring
 * of the reply string. The buffer is allocated dduring the call and should be
 * freed after use.
 * @return                // TODO: Figure out how to report error types (enums?)
 */
int recv_invalid_cmd(uint8_t *reply_str_size, char **reply_str);

#endif /* CLIENT_PROTOC_H */
