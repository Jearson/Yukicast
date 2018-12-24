#ifndef NETWORK_H
#define NETWORK_H

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
