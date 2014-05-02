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

int handle_connection(int dst_socket)
{
  char buf_in[BUF_SIZE];
  char buf_out[BUF_SIZE];
  memset(buf_in, 0, sizeof(buf_in));
  memset(buf_out, 0, sizeof(buf_in));
  int buf_written, buf_read, buf_available = 0;

  /* Read */
  buf_read = recv(dst_socket, &buf_in, sizeof(buf_in), 0);
  buf_available = BUF_SIZE - buf_read;
  parse_input(buf_in, buf_out);

  /* Write */
  buf_available = 0;
  buf_written = send(dst_socket, &buf_out, sizeof(buf_out), 0);
  buf_available = BUF_SIZE - buf_written;
  return buf_available;
}

void parse_input(char *in_msg, char *out_msg)
{
  const char *message1 = "Message understood!\n";
  const char *message2 = "Message empty!\n";
  if(strncmp(in_msg, "hola", strlen("hola")) == 0)
  {
    strncpy(out_msg, message1, strlen(message1));
  } else
  {
    strncpy(out_msg, message2, strlen(message2));
  }
}
