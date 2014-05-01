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

#include "dispatch.h"

int dispatch_input(int *dst_socket)
{
  char recv_buf[RECV_MAX];
  int recv_bytes, send_bytes;

  memset(recv_buf, 0, sizeof(recv_buf));
  recv_bytes = recv(*dst_socket, recv_buf, sizeof(recv_buf), 0);
  if(strncmp(recv_buf, "hola", strlen("hola")) == 0)
  {
    const char *message = "Has respondido hola!\n";
    send_bytes = send(*dst_socket, message, strlen(message), 0);
  } else
  {
    const char *message = "Input not found.\n";
    send_bytes = send(*dst_socket, message, strlen(message), 0);
  }
  return recv_bytes;
}
