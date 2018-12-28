#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../include/snowcast_server_protoc.h"
#include "../include/network.h"
#include "../include/debug.h"

// Wait 100ms before timing out a receive call
const struct timeval RECV_TIMEOUT = { .tv_sec = 0, .tv_usec = 100  };
const struct timeval RESET_TIMEOUT = { .tv_sec = 0, .tv_usec = 0  };

/**
 * Enables timeouts for the given socket
 * @param  sfd the socket to enable timeouts on
 * @return     0 on success, -1 on error with errno set
 */
int enable_timeout(int sfd) {
	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &RECV_TIMEOUT, 
    	sizeof(RECV_TIMEOUT)) != 0) {
		warning_fprintf(stderr, "Failed to enable timeout: %s\n", 
			strerror(errno));
    	return -1;
    }
    return 0;
}

/**
 * Disables timeouts for the given socket
 * @param  sfd the socket to disable timeouts on
 * @return     0 on success, -1 on error with errno set
 */
int disable_timeout(int sfd) {
	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO,  &RESET_TIMEOUT,
		sizeof(RESET_TIMEOUT)) != 0) {
		warning_fprintf(stderr, "Failed to disable timeout: %s\n", 
			strerror(errno));
    	return -1;
    }
    return 0;
}

/**
 * Receives the command number of a message sent by the client
 * @param  cmd_type A pointer to a cmd_type_t which will be filled with the
 * message's command number
 * @return          0 on success, -1 otw
 */
int recv_cmd_type(int sfd, uint8_t *cmd_type) {
    // Receive bytes
	int recv_res = recv_n_bytes(sfd, cmd_type, sizeof(uint8_t));
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_reply_type(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_reply_type(): %s\n", strerror(errno));
		}
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		return -1;
	}
	return 0;
}

/**
 * Receives the rest of a Hello message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg hello_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_hello(int sfd, hello_msg_t *msg) {
	// Set the timeout before receiving
    enable_timeout(sfd);

	uint16_t udp_port;
	int recv_res = recv_n_bytes(sfd, &udp_port, sizeof(udp_port));
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_hello(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_hello): %s\n", strerror(errno));
		}
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		return -1;
	}
	udp_port = ntohs(udp_port);
	msg->udp_port = udp_port;

	// Reset timeout
	disable_timeout(sfd);
	return 0;
}

/**
 * Receives the rest of a Set Station message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg set_station_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_set_station(int sfd, set_station_msg_t *msg) {
	// Set the timeout before receiving
    enable_timeout(sfd);

	uint16_t station_num;
	int recv_res = recv_n_bytes(sfd, &station_num, sizeof(station_num));
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_set_station(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_set_station(): %s\n", strerror(errno));
		}
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		return -1;
	}
	station_num = ntohs(station_num);
	msg->station_num = station_num;

	// Reset timeout
	disable_timeout(sfd);
	return 0;
}

/**
 * Sends a Welcome message to the client.
 * @param  num_stations The number of music stations the server has
 * @return              0 on success, -1 on error
 */
int send_welcome(int sfd, uint16_t num_stations) {
	char buf[WELCOME_SZ];
	welcome_msg_t msg = { 
		.reply_type = WELCOME_REPLY, 
		.num_stations = num_stations
	};

	if (marshall_welcome(&msg, &buf)) {
		error_fprintf(stderr, "send_welcome(): failed to marshall\n");
		return -1;
	}
	if (send(sfd, buf, sizeof(buf), 0) < 0) {
		error_fprintf(stderr, "send_welcome(): %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

/**
 * Sends an Announce message to the client.
 * @param  song_name_size The number of characters in the song name
 * @param  song_name      The song name string
 * @return                0 on success, -1 on error
 */
int send_announce(int sfd, uint8_t song_name_size, char *song_name) {
	int bufsz = ANNOUNCE_SZ + song_name_size;
	char *buf = malloc(bufsz);
	announce_msg_t msg = { 
		.reply_type = ANNOUNCE_REPLY, 
		.song_name_size = song_name_size,
		.song_name = song_name
	};

	if (marshall_announce(&msg, &buf)) {
		error_fprintf(stderr, "send_announce(): failed to marshall\n");
		free(buf);
		return -1;
	}
	if (send(sfd, buf, bufsz, 0) < 0) {
		error_fprintf(stderr, "send_announce(): %s\n", strerror(errno));
		free(buf);
		return -1;
	}
	free(buf);
	return 0;
}

/**
 * Sends an Invalid Command message to the client.
 * @param  reply_str_size The number of characters in the error message
 * @param  reply_str      The error message string
 * @return                0 on success, -1 on error
 */
int send_invalid_cmd(int sfd, uint8_t reply_str_size, char *reply_str) {
	int bufsz = INVALID_CMD_SZ + reply_str_size;
	char *buf = malloc(bufsz);
	invalid_cmd_msg_t msg = { 
		.reply_type = INVALID_CMD_REPLY, 
		.reply_str_size = reply_str_size,
		.reply_str = reply_str
	};

	if (marshall_invalid_cmd(&msg, &buf)) {
		error_fprintf(stderr, "send_invalid_cmd): failed to marshall\n");
		free(buf);
		return -1;
	}
	if (send(sfd, buf, bufsz, 0) < 0) {
		error_fprintf(stderr, "send_invalid_cmd(): %s\n", strerror(errno));
		free(buf);
		return -1;
	}
	free(buf);
	return 0;
}
