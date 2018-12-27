#include "../include/network.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "../include/debug.h"


/**
 * Attempts to establish a TCP connection on the given destination IP address
 * and port.
 * 
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         The connected socket descriptor on success, -1 otw
 */
int connect_tcp(char *dest_ip, char *port) {
	struct addrinfo hints, *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int gai_res = getaddrinfo(dest_ip, port, &hints, &result);
	if (gai_res != 0) {
		error_fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(gai_res));
		return -1;
	}

	int sfd;
	int connect_res;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1) {
			#ifdef DEBUG
			warning_fprintf(stderr, "socket(): %s\n", strerror(errno));
			#endif
			continue;
		}

		connect_res = connect(sfd, rp->ai_addr, rp->ai_addrlen);
		if (connect_res == 0) {
			// Success!
			break;
		}

		#ifdef DEBUG
		warning_fprintf(stderr, "connect(): %s\n", strerror(errno));
		#endif
		close(sfd);
	}

	freeaddrinfo(result);
	if (rp == NULL) {
		error_fprintf(stderr, "Exhausted getaddrinfo results\n");
   		return -1;
   	}

   	return sfd;
}

/**
 * Sets up and returns a socket descriptor from which the caller can
 * accept incoming TCP connections.
 * 
 * @param  port	The port to accept connections on
 * @return      A listening socket descriptor
 */
int setup_tcp_listen(char *port) {
	struct addrinfo hints, *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int gai_res = getaddrinfo(NULL, port, &hints, &result);
	if (gai_res != 0) {
		error_fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(gai_res));
		return -1;
	}

	int sfd, bind_res, listen_res;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd < 0) {
			#ifdef DEBUG
			warning_fprintf(stderr, "socket(): %s\n", strerror(errno));
			#endif
			continue;
		}

		bind_res = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (bind_res < 0) {
			close(sfd);
			#ifdef DEBUG
			warning_fprintf(stderr, "listen(): %s\n", strerror(errno));
			#endif
            continue;
        }

        listen_res = listen(sfd, 20);
		if (listen_res == 0) {
			// Success!
			break;
		}

		#ifdef DEBUG
		warning_fprintf(stderr, "listen(): %s\n", strerror(errno));
		#endif
		close(sfd);
	}

	freeaddrinfo(result);
	if (rp == NULL) {
		error_fprintf(stderr, "Exhausted getaddrinfo results\n");
   		return -1;
   	}

   	return sfd;
}

/**
 * Attempts to make a UDP 'connection' to the given destination IP address
 * and port. Doing this allows for dest_addr and dest_len to be unspecified
 * in sendto() calls as well as prevent unecessary route lookups when using
 * sendmsg().
 * 
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         The connected socket descriptor on success, -1 otw
 */
int connect_udp(char *dest_ip, char *port) {
	struct addrinfo hints, *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int gai_res = getaddrinfo(dest_ip, port, &hints, &result);
	if (gai_res != 0) {
		error_fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(gai_res));
		return -1;
	}

	int sfd;
	int connect_res;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1) {
			#ifdef DEBUG
			warning_fprintf(stderr, "socket(): %s\n", strerror(errno));
			#endif
			continue;
		}

		connect_res = connect(sfd, rp->ai_addr, rp->ai_addrlen);
		if (connect_res == 0) {
			// Success!
			break;
		}

		#ifdef DEBUG
		warning_fprintf(stderr, "connect(): %s\n", strerror(errno));
		#endif
		close(sfd);
	}

	freeaddrinfo(result);
	if (rp == NULL) {
		error_fprintf(stderr, "Exhausted getaddrinfo results\n");
   		return -1;
   	}
   	
   	return sfd;
}

/**
 * Opens up a local port, binds it, and returns a socket descriptor from which
 * data can be read from.
 * 
 * @param  port    The port to bind on
 * @return         A socket descriptor on success, -1 otw
 */
int bind_udp(char *port) {
	struct addrinfo hints, *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	int gai_res = getaddrinfo(NULL, port, &hints, &result);
	if (gai_res != 0) {
		error_fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(gai_res));
		return -1;
	}

	int sfd;
	int bind_res;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd < 0) {
			#ifdef DEBUG
			warning_fprintf(stderr, "socket(): %s\n", strerror(errno));
			#endif
			continue;
		} 

		bind_res = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (bind_res == 0) {
			// Success!
			break;
		}

		#ifdef DEBUG
		warning_fprintf(stderr, "bind(): %s\n", strerror(errno));
		#endif
		close(sfd);
	}

	freeaddrinfo(result);
	if (rp == NULL) {
		error_fprintf(stderr, "Exhausted getaddrinfo results\n");
		return -1;
	}
	return sfd;
}
