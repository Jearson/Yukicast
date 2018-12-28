#ifndef NETWORK_H
#define NETWORK_H

/**
 * Wrapper around recv that ensures n bytes are received from the given
 * socket, otherwise a 0 or -1 is reported on close or error, respectively.
 * @param  sockfd the socket to receive on
 * @param  buf  buffer of size n where bytes will be written to
 * @param  n      the number of bytes to receive
 * @return        n on success, 0 if the conneciton had been closed, or 
 * -1 on error
 */
int recv_n_bytes(int sockfd, void *bytes, int n);

/**
 * Establishes a TCP connection to the given destination IP address
 * and port.
 * 
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         The connected socket descriptor on success, -1 otw
 */
int connect_tcp(char *dest_ip, char *port);
/**
 * Sets up and returns a socket descriptor from which the caller can
 * accept incoming TCP connections.
 * 
 * @param  port	The port to accept connections on
 * @return      A listening socket descriptor
 */
int setup_tcp_listen(char *port);
/**
 * Attempts to make a 'connected' UDP to the given destination IP address
 * and port.
 * 
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         The connected socket descriptor on success, -1 otw
 */
int connect_udp(char *dest_ip, char *port);
/**
 * Opens up a local port, binds it, and returns a socket descriptor from which
 * data can be read from.
 * 
 * @param  port    The port to bind on
 * @return         A socket descriptor on success, -1 otw
 */
int bind_udp(char *port);

#endif /* NETWORK_H */
