#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

#include "../../include/snowcast_control_protoc.h"
#include "../../include/snowcast_control.h"
#include "../../include/network.h"
#include "../../include/debug.h"

pthread_t cli_thr;
pthread_t server_listener_thr;
int sfd;

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "./snowcast_control "
			"<servername> <serverport> <udpport>\n");
		return -1;
	}

	char *servname = argv[1], *servport = argv[2], *udpport = argv[3];
	int udp_port = strtol(udpport, NULL, 10);
	// TCP connect to the server
	info_fprintf(stderr, "Connecting to server %s:%s\n", servname, servport);
	if ((sfd = connect_tcp(servname, servport)) < 0) {
		fatal_fprintf(stderr, "Failed to establish server connection\n");
		return -1;
	}
	success_fprintf(stderr, "Established TCP connection to %s:%s\n", 
				servname, servport);
    
    // Start server messsage listener thread, which will start the cli thread
    // after receving the welcome reply
	if (pthread_create(
		&server_listener_thr, NULL, &server_msg_listen, &udp_port) != 0) {
		fatal_fprintf(stderr, "Failed to start listener for server messages\n");
		return -1;
	}

	pthread_join(cli_thr, NULL);
	pthread_join(server_listener_thr, NULL);
	fprintf(stdout, "Goodbye!\n");	
	return 0;
}

void *do_cli(void *arg) {

	int run = 1;
	char buf[16];
	int station_num = -1;
	char *strtol_ptr;

	// If the user gives more than sizeof(buf)-1 chars, 
	// the body of the while loop executes more than once
	while (run) {
		memset(buf, 0, sizeof(buf));
		if (fgets(buf, sizeof(buf), stdin)) {
			switch (buf[0]) {
				case '\n':
				// do nothing
				break;
				case 'q':
				run = 0;
				break;
				default:
					// Try to interpret the string as a number
					station_num = strtol(buf, &strtol_ptr, 10);
					if (station_num == 0 && errno != 0) {
						error_fprintf(stderr, "%s\n", strerror(station_num));
						errno = 0;
					} else if (strtol_ptr == buf) {
						error_fprintf(stderr, "Invalid command\n");
					} else {
						// Send Set Station message
						if (send_set_station(sfd, station_num) < 0) {
							error_fprintf(stderr, "Failed to send Set "
								"Station message\n");
							run = 0;
						}
					}
			}
			station_num = -1;
		} else {
			run = 0;
		}
	}
	pthread_cancel(server_listener_thr);
	return NULL;
}

/**
 * For listening and acting on messages sent by the server
 * @param  arg (unused)
 * @return     (unused)
 */
void *server_msg_listen(void *arg) {
	int udp_port = *(int *) arg;
	// Hello the server 
	if (send_hello(sfd, udp_port) < 0) {
		error_fprintf(stderr, "Failed to send Hello\n");
		return NULL;
	}
	info_fprintf(stderr, "Hello Sent. Waiting for Welcome...\n");

	welcome_msg_t welcome;
	announce_msg_t announce;
	invalid_cmd_msg_t invalid_cmd;

	int welcome_recvd = 0, run = 1;
	uint8_t reply_type;
	uint16_t num_stations;
	while (run) {
		if (recv_reply_type(sfd, &reply_type) < 0) {
			return NULL;
		}
		switch (reply_type) {
			case WELCOME_REPLY:
				memset(&welcome, 0, WELCOME_SZ);
				if (welcome_recvd) {
					warning_fprintf(stderr, "Gratuitous welcome received. "
						"Reading in and ignoring...\n");
					if (recv_rest_welcome(sfd, &welcome) < 0) {
						error_fprintf(stderr, "Failed to receive welcome message\n");
						run = 0;
					}
				} else {
					if (recv_rest_welcome(sfd, &welcome) < 0) {
						error_fprintf(stderr, "Failed to receive welcome message\n");
						run = 0;
					}
					num_stations = welcome.num_stations;
					info_fprintf(stdout, "There are %d stations.\n", num_stations);
					welcome_recvd = 1;
					// Begin the CLI thread
					if (pthread_create(&cli_thr, NULL, &do_cli, &udp_port) != 0) {
						fatal_fprintf(stderr, "Failed to start CLI thread\n");
						run = 0;
						break;
					}
				}
			break;
			case ANNOUNCE_REPLY:
				memset(&announce, 0, ANNOUNCE_SZ);
				if (recv_rest_announce(sfd, &announce) < 0) {
					error_fprintf(stderr, "Failed to receive announce message\n");
					run = 0;
				}
				fprintf(stdout, "Listening to: '%s'\n", announce.song_name);
			break;
			case INVALID_CMD_REPLY:
				memset(&invalid_cmd, 0, INVALID_CMD_SZ);
				if (recv_rest_invalid_cmd(sfd, &invalid_cmd) < 0) {
					error_fprintf(stderr, "Failed to receive invalid "
						"command message\n");
					run = 0;
				} else {
					error_fprintf(stderr, "%s\n", invalid_cmd.reply_str);
					run = 0;
				}
			break;
			default:
				error_fprintf(stderr, "Invalid reply received: %d\n", reply_type);
		}
	}
	
	pthread_cancel(cli_thr);
	return NULL;
}
