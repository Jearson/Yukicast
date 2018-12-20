#include "../include/network.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>

#include "../include/debug.h"


// returns a connected tcp socket to the dest_ip:port
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
			success_fprintf(stderr, "Established connection to %s:%s\n", 
				dest_ip, port);
			break;
		}

		#ifdef DEBUG
		warning_fprintf(stderr, "connect(): %s\n", strerror(errno));
		#endif
		close(sfd);
	}

	if (rp == NULL) {
		error_fprintf(stderr, "Could not connect to %s:%s\n", dest_ip, port);
   		return -1;
   	}

   	freeaddrinfo(result);
   	return sfd;
}

