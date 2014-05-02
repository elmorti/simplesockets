/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#include <stdio.h>

#include "server.h"

int main()
{
  int srv_socket = start_server();
  run_server(srv_socket);
  return 0;
}
