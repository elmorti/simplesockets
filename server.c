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
#include "server.h"

/* Initializes server address and listening socket */
void setup_server(struct sockaddr_in* srv_sockaddr, int *srv_socket)
{

  /* Initialize the struct to zero */
  if (memset(srv_sockaddr, 0, sizeof(*srv_sockaddr)) == NULL)
  {
    perror("Error memset.\n");
    exit(1);
  }

  /* Fill up the struct */
  srv_sockaddr->sin_family = AF_INET;
  srv_sockaddr->sin_port = htons(DEFAULT_PORT);
  srv_sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);

  /* Initialize the listening socket */
  if((*srv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Socket error.\n");
    exit(1);
  }
}

/* Starts server */
void start_server(struct sockaddr_in* srv_sockaddr, int *srv_socket)
{
  if(bind(*srv_socket, (struct sockaddr *)srv_sockaddr, sizeof(struct sockaddr)) == -1)
  {
    perror("Error bind.\n");
    exit(1);
  }
  if(listen(*srv_socket,MAX_CONN) == -1)
  {
    perror("Error listen.\n");
  }
  printf("Server started... Socket %d\n", *srv_socket);
}

int main()
{
  
  setup_server(&srv_sockaddr, &srv_socket);
  start_server(&srv_sockaddr, &srv_socket);

  addr_size = sizeof(dst_sockaddr);
  while (1)
  {
    int dst_socket = accept(srv_socket, (struct sockaddr *)&dst_sockaddr, &addr_size);
    printf("Accepting connection for socket: %d\n",dst_socket);
    dispatch_input(&dst_socket);
    //close(dst_socket);
  }
  // Free resources
  close(srv_socket);
  return 0;
}
