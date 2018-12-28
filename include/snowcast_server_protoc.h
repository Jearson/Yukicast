#ifndef SNOWCAST_SERVER_PROTOC_H
#define SNOWCAST_SERVER_PROTOC_H

#include <sys/time.h>
#include "snowcast_msgs.h"

/**
 * Receives the command number of a message sent by the client
 * @param  cmd_type A pointer to a cmd_type_t which will be filled with the
 * message's command number
 * @return          // TODO: Figure out how to report error types (enums?)
 */
int recv_cmd_type(uint8_t *cmd_type);
/**
 * Receives a Hello message from the client.
 * @param  udp_port The port to send song data to
 * @return           // TODO: Figure out how to report error types (enums?)
 */
int recv_hello(uint16_t *udp_port);
/**
 * Receives a Set Station message from the client.
 * @param  station_num Pointer to uint16_t that will store the number of 
 * stations the server has as specified by the message
 * @return              // TODO: Figure out how to report error types (enums?)
 */
int recv_set_station(uint16_t *station_num);
/**
 * Receives the rest of a Hello message from the server.
 * @param  udp_port The port to send song data to
 * @return           // TODO: Figure out how to report error types (enums?)
 */
int recv_rest_hello(uint16_t *udp_port);
/**
 * Receives the rest of a Set Station message from the client.
 * @param  station_num Pointer to uint16_t that will store the number of 
 * stations the server has as specified by the message
 * @return              // TODO: Figure out how to report error types (enums?)
 */
int recv_rest_set_station(uint16_t *station_num);
/**
 * Sends a Welcome message to the client.
 * @param  num_stations The number of music stations the server has
 * @return              // TODO: Figure out how to report error types (enums?)
 */
int send_welcome(uint16_t num_stations);
/**
 * Sends an Announce message to the client.
 * @param  song_name_size The number of characters in the song name
 * @param  song_name      The song name string
 * @return                // TODO: Figure out how to report error types (enums?)
 */
int send_announce(uint8_t song_name_size, char *song_name);
/**
 * Sends an Invalid Command message to the client.
 * @param  reply_str_size The number of characters in the error message
 * @param  reply_str      The error message string
 * @return                // TODO: Figure out how to report error types (enums?)
 */
int send_invalid_cmd(uint8_t reply_str_size, char *reply_str);

#endif /* SNOWCAST_SERVER_PROTOC_H */
