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

#define RECV_MAX      1024
#define DEFAULT_PORT  5432
#define MAX_CONN      10

fd_set read_fd_set;   /* Sockets ready for read() */
fd_set active_fd_set; /* Sockets active */

struct sockaddr_in srv_sockaddr;  /* Server address info */
struct sockaddr_in dst_sockaddr;  /* Client address info */

socklen_t dst_addrsize;  /* Client Address length */

int srv_socket; /* Server listening socket */
int dst_socket; /* Client socket */

/* Function declarations */
void setup_server(struct sockaddr_in*, int*);
void start_server(struct sockaddr_in*, int*);

#endif /* server.h */
