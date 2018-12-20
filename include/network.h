#ifndef NETWORK_H
#define NETWORK_H

/**
 * Attempts to establish a TCP connection on the given destination IP address
 * and port.
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         The connected socket descriptor on success, -1 otw
 */
int connect_tcp(char *dest_ip, char *port);
/**
 * Attempts to make a 'connected' UDP to the given destination IP address
 * and port.
 * @param  dest_ip The destination IP
 * @param  port    The destination port
 * @return         The connected socket descriptor on success, -1 otw
 */
int connect_udp(char *dest_ip, char *port);
/**
 * Opens up a local port, binds it, and returns a socket descriptor from which
 * data can be read from.
 * @param  port    The port to bind on
 * @return         A socket descriptor on success, -1 otw
 */
int bind_udp(char *port);

#endif /* NETWORK_H */
