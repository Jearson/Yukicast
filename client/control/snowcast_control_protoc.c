#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../../include/snowcast_control_protoc.h"
#include "../../include/network.h"
#include "../../include/debug.h"

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
 * Sends a hello message to the server
 * @param  udp_port the udp port to send song data to
 * @return          	0 on success, -1 on error
 */
int send_hello(int sfd, uint16_t udp_port) {
	char buf[HELLO_SZ];
	hello_msg_t msg = { 
		.cmd_type = HELLO_CMD, 
		.udp_port = udp_port 
	};
	
	if (marshall_hello(&msg, &buf)) {
		error_fprintf(stderr, "send_hello(): failed to marshall\n");
		return -1;
	}
	if (send(sfd, buf, sizeof(buf), 0) < 0) {
		error_fprintf(stderr, "send_hello(): %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

/**
 * Sends a set station message to the server
 * @param  station_num the station number the client wants to listen to
 * @return         		0 on success, -1 on error
 */
int send_set_station(int sfd, uint16_t station_num) {
	char buf[SET_STATION_SZ];
	set_station_msg_t msg = { 
		.cmd_type = SET_STATION_CMD, 
		.station_num = station_num
	};
	
	if (marshall_set_station(&msg, &buf)) {
		error_fprintf(stderr, "send_set_station(): failed to marshall\n");
		return -1;
	}
	if (send(sfd, buf, sizeof(buf), 0) < 0) {
		error_fprintf(stderr, "send_set_station(): %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

/**
 * Receives the reply number of a message sent by the server
 * @param  reply_type A pointer to a uint8_t that will be filled with a 
 * reply number: 0 for Welcome, 1 for Announce, and 2 for Invalid Command
 * @return             0 on success, -1 otw
 */
int recv_reply_type(int sfd, uint8_t *reply_type) {
    // Receive bytes
	int recv_res = recv_n_bytes(sfd, reply_type, sizeof(uint8_t));
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
 * Receives the rest of a Welcome message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg welcome_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_welcome(int sfd, welcome_msg_t *msg) {
	// Set the timeout before receiving
    enable_timeout(sfd);

	uint16_t num_stations;
	int recv_res = recv_n_bytes(sfd, &num_stations, sizeof(num_stations));
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_welcome(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_welcome(): %s\n", strerror(errno));
		}
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		return -1;
	}
	num_stations = ntohs(num_stations);
	msg->num_stations = num_stations;

	// Reset timeout
	disable_timeout(sfd);
	return 0;
}

/**
 * Receives the rest of an Announce from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg announce_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_announce(int sfd, announce_msg_t *msg) {
	// Set the timeout before receiving
    enable_timeout(sfd);

	uint8_t song_name_size;
	char *song_name;

	// Receive the size of the song name
	int recv_res = recv_n_bytes(sfd, &song_name_size, sizeof(song_name_size));
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_announce(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_announce(): %s\n", strerror(errno));
		}
		disable_timeout(sfd);
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		disable_timeout(sfd);
		return -1;
	}

	song_name = malloc(song_name_size);
	if (song_name == NULL) {
		error_fprintf(stderr, "recv_rest_announce(): %s\n", strerror(errno));
	}

	// Receive the song name
	recv_res = recv_n_bytes(sfd, song_name, song_name_size);
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_announce(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_announce(): %s\n", strerror(errno));
		}
		disable_timeout(sfd);
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		disable_timeout(sfd);
		return -1;
	}
	msg->song_name_size = song_name_size;
	msg->song_name = song_name;

	// Reset timeout
	disable_timeout(sfd);
	return 0;
}

/**
 * Receives the rest of an Invalid Command message from the server.
 * @param  sfd the socket to receive a message on
 * @param  msg invalid_cmd_msg_t to fill
 * @return     0 on success, -1 otw
 */
int recv_rest_invalid_cmd(int sfd, invalid_cmd_msg_t *msg) {
	// Set the timeout before receiving
    enable_timeout(sfd);

	uint8_t reply_str_size;
	char *reply_str;

	// Receive the size of the reply string
	int recv_res = recv_n_bytes(sfd, &reply_str_size, sizeof(reply_str_size));
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_announce(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_announce(): %s\n", strerror(errno));
		}
		disable_timeout(sfd);
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		disable_timeout(sfd);
		return -1;
	}

	reply_str = malloc(reply_str_size);
	if (reply_str == NULL) {
		error_fprintf(stderr, "recv_rest_announce(): %s\n", strerror(errno));
	}

	// Receive the reply string
	recv_res = recv_n_bytes(sfd, reply_str, reply_str_size);
	if (recv_res < 0) {
		if (errno == EAGAIN) {
			error_fprintf(stderr, "recv_rest_announce(): Receive timed out\n");
		} else {
			error_fprintf(stderr, "recv_rest_announce(): %s\n", strerror(errno));
		}
		disable_timeout(sfd);
		return -1;
	} else if (recv_res == 0) {
		error_fprintf(stderr, "Connection closed by the other side\n");
		disable_timeout(sfd);
		return -1;
	}
	msg->reply_str_size = reply_str_size;
	msg->reply_str = reply_str;

	// Reset timeout
	disable_timeout(sfd);
	return 0;
}
