/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dispatch.h"
#include "server.h"

/* Initializes server address and listening socket */
void setup_server(struct sockaddr_in* srv_sockaddr, int *srv_socket)
{

  /* Initialize the struct to zero */
  if (memset(srv_sockaddr, 0, sizeof(*srv_sockaddr)) == NULL)
  {
    perror("memset() failed");
    exit(1);
  }

  /* Fill up the struct */
  srv_sockaddr->sin_family = AF_INET;
  srv_sockaddr->sin_port = htons(DEFAULT_PORT);
  srv_sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);

  /* Initialize the listening socket */
  *srv_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(*srv_socket == -1)
  {
    perror("socket() failed");
    exit(1);
  }
  if(fcntl(*srv_socket, F_SETFL, O_NONBLOCK) == -1)
  {
    perror("fcntl() failed");
  }
}

/* Starts server */
void start_server(struct sockaddr_in* srv_sockaddr, int *srv_socket)
{
  if(bind(*srv_socket, 
          (struct sockaddr *)srv_sockaddr, 
          sizeof(*srv_sockaddr)) == -1)
  {
    perror("bind() failed");
    exit(1);
  }
  if(listen(*srv_socket,MAX_CONN) == -1)
  {
    perror("listen() failed");
  }
  printf("Server started... Socket %d\n", *srv_socket);
}

int main()
{
  int i;
  int max_fd;
  const char *socket_optval = "yes";
  
  setup_server(&srv_sockaddr, &srv_socket);
  start_server(&srv_sockaddr, &srv_socket);

  /* Insert the server socket onto the sockets active set */
  FD_ZERO(&active_fd_set);
  FD_ZERO(&read_fd_set);
  FD_SET(srv_socket, &active_fd_set);
  /* Max FD is actually the server socket */
  max_fd = srv_socket;

  if(setsockopt(srv_socket,
                SOL_SOCKET,
                SO_REUSEADDR,
                (char *)&socket_optval,
                sizeof(socket_optval)) == -1)
  {
    perror("setsockopt() failed");
    exit(1);
  }

  /* Main loop */
  while (1)
  {
    /* Make a copy of the active set */
    FD_ZERO(&read_fd_set);
    read_fd_set = active_fd_set;

    /* select() Wait for socket activity */
    int sockets = select(max_fd+1, &read_fd_set, NULL, NULL, NULL);
    if(sockets == -1)
    {
      perror("select() failed");
      exit(1);
    } else if(sockets > 0)

    {
      printf("select() succeeded %d sockets ready.\n",sockets);
    }

    if(FD_ISSET(srv_socket, &read_fd_set))
    {
      /* If it is our srv_socket which is ready for read, then process */
      printf("New connection incoming on %d.\n",srv_socket);
      dst_socket = accept(srv_socket, 
                         (struct sockaddr *)&dst_sockaddr,
                          &dst_addrsize);
      if(dst_socket == -1)
      {
        perror("socket() failed");
        exit(1);
      }
        
      /* So we have a new connection, put it in the active set */
      printf("Accepting new connection at socket %d.\n", dst_socket);
      FD_SET(dst_socket, &active_fd_set);
      if(fcntl(dst_socket, F_SETFL, O_NONBLOCK) == -1)
      {
        perror("fcntl()");
      }

      if (dst_socket > max_fd)
      {
        max_fd = dst_socket;
      }
    }

    /* There is activity, loop over the fd set and process them. */
    for (i = 0; i <= max_fd; i++)
    {
      /* Skip server socket as we already accepted */
      if (i == srv_socket)
      {
        continue;
      }

      if(FD_ISSET(i, &read_fd_set))
      {

        /* We process the data and close the socket and clear it */
        printf("Dispatching active connection %d.\n",i);
        int n = dispatch_input(&i, max_fd);
        if(n <= 0)
        {
          FD_CLR(i, &active_fd_set);
          close(i);
        }
      } 
    }
  }
  return 0;
}
