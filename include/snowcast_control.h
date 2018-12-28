#ifndef SNOWCAST_CLIENT_H
#define SNOWCAST_CLIENT_H

/**
 * Snowcast Control Client header file
 */

/**
 * For listening and acting on messages sent by the server
 * @param  arg (unused)
 * @return     (unused)
 */
void *server_msg_listen(void *arg);

/**
 * For the command line interface of the program
 * @param  arg [description]
 * @return     [description]
 */
void *do_cli(void *arg);
#endif /* SNOWCAST_CLIENT_H */
