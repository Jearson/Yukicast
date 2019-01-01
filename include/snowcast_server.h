#ifndef SNOWCAST_SERVER_H
#define SNOWCAST_SERVER_H

/**
 * Snowcast Server header file
 */
void *do_cli(void *arg);
void *accept_connections(void *arg);
void *serve_client(void *arg);
void *stream_data(void *arg);
void print_stations();
void cleanup();

#endif /* SNOWCAST_SERVER_H */
