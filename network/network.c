#include "../include/network.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>

#include "../include/debug.h"


/**
 * Attempts to establish a TCP connection on the given destination IP address
 * and port.
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

	/* 
		getaddrinfo() returns a list of address structs. 
		We try each address until we successfully connect().
		If either socket() or connect() fails, we close the 
		socket and try the next address struct. 
	*/
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
		error_fprintf(stderr, "Exhausted all getaddrinfo results\n");
   		return -1;
   	}

   	return sfd;
}

/**
 * Attempts to make a 'connected' UDP to the given destination IP address
 * and port.
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

	/* 
		getaddrinfo() returns a list of address structs. 
		We try each address until we successfully connect().
		If either socket() or connect() fails, we close the 
		socket and try the next address struct. 
	*/
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
		error_fprintf(stderr, "Exhausted all getaddrinfo results\n");
   		return -1;
   	}
   	
   	return sfd;
}

/**
 * Attempts to return a UDP socket for the given IP address and port.
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         A socket descriptor on success, -1 otw
 */
int setup_udp_socket(char *dest_ip, char *port) {
	struct addrinfo hints, *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int gai_res = getaddrinfo(dest_ip, port, &hints, &result);
	if (gai_res != 0) {
		error_fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(gai_res));
		return -1;
	}

	/* 
		getaddrinfo() returns a list of address structs. 
		We try each address until we successfully connect().
		If either socket() or connect() fails, we close the 
		socket and try the next address struct. 
	*/
	int sfd;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd != -1) {
			// Success!
			freeaddrinfo(result);
			return sfd;
		} 
		#ifdef DEBUG
		warning_fprintf(stderr, "socket(): %s\n", strerror(errno));
		#endif
	}

	freeaddrinfo(result);
	error_fprintf(stderr, "Exhausted all getaddrinfo results\n");
	return -1;
}
