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

#include "dispatcher.h"

int handle_readfd(char *buf, int readfd)
{
  buf = (char *) malloc(BUF_SIZE * sizeof(char));
  memset(&buf, 0, sizeof(buf));
  int buf_read, buf_available = 0;

  /* TODO(spartida): Implement read_all */
  buf_read = recv(readfd, &buf, sizeof(buf), 0);
  if (buf_read < 0)
  {
    if(errno == EWOULDBLOCK)
    {
      printf("(Socked %d): Got EWOULDBLOCK.\n",readfd);
    }
    perror("recv() failed");
  }
  buf_available = BUF_SIZE - buf_read;
  printf("(Socket %d): Read %d bytes, buffer available %d bytes.\n",
         readfd,
         buf_read,
         buf_available);
  return buf_read;
}

int handle_writefd(char *message,
                   int srv_socket,
                   int max_fd,
                   fd_set *active_fd_set)
{
  char buf[BUF_SIZE];
  memset(buf, 0, sizeof(buf));
  int i, buf_written, buf_available = 0;


  /* TODO(spartida): Implement send_all */
  for (i = 0; i<=max_fd; i++)
  {
    if(i == srv_socket)
    {
      continue;
    }
    if(FD_ISSET(i, active_fd_set))
    {
      sprintf(buf, "(Socket %d): %s\n", i, message);
      buf_written = send(i, &buf, sizeof(buf), 0);
      if (buf_written < 0)
      {
        perror("send() failed");
        return -1;
      }
      buf_available = BUF_SIZE - buf_written;
      printf("Written %d bytes, buffer available %d bytes.\n",
             buf_written,
             buf_available);
    }
  }
  return buf_written;
}
