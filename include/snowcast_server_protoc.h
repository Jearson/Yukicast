#ifndef SNOWCAST_SERVER_PROTOC_H
#define SNOWCAST_SERVER_PROTOC_H

#include <sys/time.h>
#include "snowcast_msgs.h"

/**
 * Receives the command number of a message sent by the client
 * @param  cmd_type A pointer to a cmd_type_t which will be filled with the
 * message's command number
 * @return          0 on success, -1 otw
 */
int recv_cmd_type(int sfd, uint8_t *cmd_type);
/**
 * Receives the rest of a Hello message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg hello_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_hello(int sfd, hello_msg_t *msg);
/**
 * Receives the rest of a Set Station message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg set_station_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_set_station(int sfd, set_station_msg_t *msg);
/**
 * Sends a Welcome message to the client.
 * @param  num_stations The number of music stations the server has
 * @return              0 on success, -1 on error
 */
int send_welcome(int sfd, uint16_t num_stations);
/**
 * Sends an Announce message to the client.
 * @param  song_name_size The number of characters in the song name
 * @param  song_name      The song name string
 * @return                0 on success, -1 on error
 */
int send_announce(int sfd, uint8_t song_name_size, char *song_name);
/**
 * Sends an Invalid Command message to the client.
 * @param  reply_str_size The number of characters in the error message
 * @param  reply_str      The error message string
 * @return                0 on success, -1 on error
 */
int send_invalid_cmd(int sfd, uint8_t reply_str_size, char *reply_str);

#endif /* SNOWCAST_SERVER_PROTOC_H */
