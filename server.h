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

/* Server address info */
struct sockaddr_in srv_sockaddr;

/* Client address info */
struct sockaddr_in dst_sockaddr;

/* Address length */
socklen_t addr_size;

/* Server listening socket */
int srv_socket;

void setup_server(struct sockaddr_in*, int*);
void start_server(struct sockaddr_in*, int*);

#endif /* server.h */
