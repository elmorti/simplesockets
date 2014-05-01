/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>

#define RECV_MAX 1024

#ifndef DISPATCH_H
#define DISPATCH_H

int dispatch_input(int *, int);

#endif /* dispatch.h */
