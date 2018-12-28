#ifndef SNOWCAST_CONTROL_PROTOC_H
#define SNOWCAST_CONTROL_PROTOC_H

#include <sys/time.h>
#include "snowcast_msgs.h"

/**
 * Sends a hello message to the server
 * @param  udp_port the udp port to send song data to
 * @return          	0 on success, -1 on error
 */
int send_hello(int sockfd, uint16_t udp_port);

/**
 * Sends a set station message to the server
 * @param  station_num the station number the client wants to listen to
 * @return         		0 on success, -1 on error
 */
int send_set_station(int sockfd, uint16_t station_num);

/**
 * Receives the reply number of a message sent by the server
 * @param  reply_type A pointer to a uint8_t that will be filled with a 
 * reply number: 0 for Welcome, 1 for Announce, and 2 for Invalid Command
 * @return             0 on success, -1 otw
 */
int recv_reply_type(int sockfd, uint8_t *reply_type);

/**
 * Receives the rest of a Welcome message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg welcome_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_welcome(int sockfd, welcome_msg_t *msg);

/**
 * Receives the rest of an Announce from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg announce_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_announce(int sockfd, announce_msg_t *msg);

/**
 * Receives the rest of an Invalid Command message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg invalid_cmd_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_invalid_cmd(int sockfd, invalid_cmd_msg_t *msg);

#endif /* SNOWCAST_CONTROL_PROTOC_H */
