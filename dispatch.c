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

int dispatch_input(int *dst_socket, int max_fd)
{
  char recv_buf[RECV_MAX];
  int i, recv_bytes, send_bytes;

  printf("%d\n",max_fd);
  memset(recv_buf, 0, RECV_MAX);
  recv_bytes = recv(*dst_socket, recv_buf, sizeof(recv_buf), 0);
  for (i = 4; i <= max_fd; i++)
  {
    send_bytes = send(i, recv_buf, sizeof(recv_buf), 0);
    if(send_bytes <= 0)
    {
      perror("send() failed");
    }
  }
  return recv_bytes;
}
