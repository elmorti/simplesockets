/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#include <sys/socket.h>

#define BUF_SIZE 1024

#ifndef DISPATCHER_H
#define DISPATCHER_H

int handle_readfd(char *, int);
int handle_writefd(char *, int, int, fd_set*);
#endif /* dispatcher.h */
