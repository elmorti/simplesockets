/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#define BUF_SIZE 1024

#ifndef DISPATCHER_H
#define DISPATCHER_H

int handle_connection(int dst_socket);
void parse_input(char*, char*);
#endif /* dispatcher.h */
