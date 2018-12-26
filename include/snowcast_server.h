#ifndef SNOWCAST_SERVER_H
#define SNOWCAST_SERVER_H

/**
 * Snowcast Server header file
 */
void *accept_connections(void *args);
void print_stations();
void cleanup();

#endif /* SNOWCAST_SERVER_H */
