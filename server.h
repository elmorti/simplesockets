/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#ifndef SERVER_H
#define SERVER_H

#define BUF_SIZE      1024
#define LISTEN_PORT   5432
#define MAX_CONN      10

/* Function declarations */
int   start_server      ();
int   run_server        (int);
int   shutdown_server   (int, int, fd_set*);
int   shutdown_socket   (int, fd_set*);

#endif /* server.h */
