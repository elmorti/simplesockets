/*
 * Author: samuel.partida@gmail.com (Samuel Partida) 2014
 * Based on many Unix C Sockets books and literature on the webz.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dispatcher.h"
#include "server.h"

/*
 * Initializes a listening socket and if successfully returns
 * its descriptor or if error returns -1
 */
int start_server()
{
  struct sockaddr_in srv_sockaddr;
  const char *socket_optval = "yes";
  int srv_socket;
  int ret = 0;

  if (memset(&srv_sockaddr, 0, sizeof(srv_sockaddr)) == NULL)
  {
    perror("memset() failed");
  }

  srv_sockaddr.sin_family = AF_INET;
  srv_sockaddr.sin_port = htons(LISTEN_PORT);
  srv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  srv_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(srv_socket == -1)
  {
    perror("socket() failed");
    ret = -1;
  }

  if(setsockopt(srv_socket,
                SOL_SOCKET,
                SO_REUSEADDR,
                (char *)&socket_optval,
                sizeof(socket_optval)) == -1)
  {
    perror("setsockopt() failed");
    ret = -1;
  }
  if(fcntl(srv_socket, F_SETFL, O_NONBLOCK) == -1)
  {
    perror("fcntl() failed");
    ret = -1;
  }

  if(bind(srv_socket, 
          (struct sockaddr *)&srv_sockaddr, 
          sizeof(srv_sockaddr)) == -1)
  {
    perror("bind() failed");
    ret = -1;
  }
  if(listen(srv_socket,MAX_CONN) == -1)
  {
    perror("listen() failed");
    ret = -1;
  }
  printf("(Socket %d): Server listening on... %s:%d\n",
         srv_socket,
         inet_ntoa(srv_sockaddr.sin_addr),
         ntohs(srv_sockaddr.sin_port));
  return srv_socket;
}

/*
 * Loops over active sockets and dispatches them
 */
int run_server(int srv_socket)
{
  int max_fd = -1;
  int act = -1;
  int i = 0;
  int dst_socket = -1;
  if(srv_socket < 0)
  {
    printf("Invalid listening socket!\n");
    return -1;
  }

  fd_set read_fd_set, active_fd_set;

  FD_ZERO(&read_fd_set);
  FD_ZERO(&active_fd_set);
  FD_SET(srv_socket, &active_fd_set);

  if(srv_socket > max_fd)
  {
    max_fd = srv_socket;
  }

  while(1)
  {
    /* Copy over descriptors of interest */
    memcpy(&read_fd_set, &active_fd_set, sizeof(active_fd_set));

    act = select(max_fd + 1, &read_fd_set, NULL, NULL, NULL);
    
    if(act < 0)
    {
      perror("select() failed");
      return -1;
    }

    /* Readfds */
    for (i = 0; i <= max_fd; i++)
    {
      if(FD_ISSET(i, &read_fd_set))
      {
        if(i == srv_socket)
        {
          dst_socket = accept(srv_socket, NULL, NULL);
          if (dst_socket == -1)
          {
            perror("accept() failed");
            continue;
          } else
          {
            FD_SET(dst_socket, &active_fd_set);

            if(dst_socket > max_fd)
            {
              max_fd = dst_socket;
            }

            printf("(Socket %d): Connection accepted.\n", dst_socket);
          }

        } else
        {
          /* TODO(spartida): Rewrite conditionals better. */
          char message[BUF_SIZE];
          dst_socket = i;
          int n = handle_readfd(message, dst_socket);
          printf("(Socket %d): Received %d bytes.\n",dst_socket,n);
          handle_writefd(message, srv_socket, max_fd, &active_fd_set);
          if(n == 0)
          {
            printf("(Socket %d): Connection closed.\n", dst_socket);
            shutdown_socket(dst_socket, &active_fd_set);
          }
        }
      }
    }
  }
  return 0;
}

int shutdown_socket(int dst_socket, fd_set *active_fd_set)
{
  /* TODO(spartida): Check pending writes and reads before closing. */
  if(dst_socket <0 || active_fd_set == NULL)
  {
    printf("Socket or socket set incorrect.\n");
    return -1;
  }

  if(FD_ISSET(dst_socket, active_fd_set))
  {
    FD_CLR(dst_socket, active_fd_set);
  }
  close(dst_socket);
  printf("(Socket %d): Successfully closed client socket.\n", dst_socket);
  return 0;
}

/*
 * Properly checks for active connections and shuts them down
 * closing the server at the end.
 *
 * TODO(spartida): Not sure how to use this yet
 */
int shutdown_server(int srv_socket, int max_fd, fd_set *active_fd_set)
{
  int i;
  for (i = 0; i <= max_fd; i++)
  {
    if(i == srv_socket)
    {
      continue;
    } else if(FD_ISSET(i, active_fd_set))
    {
      shutdown_socket(i, active_fd_set);
    }
  }

  if(FD_ISSET(srv_socket, active_fd_set))
  {
    FD_CLR(srv_socket, active_fd_set);
  }
  close(srv_socket);
  printf("(Socket %d): Successfully closed server socket.\n", srv_socket);
  return 0;
}
