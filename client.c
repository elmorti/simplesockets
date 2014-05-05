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
#include <netdb.h>
#include <fcntl.h>

#define SEND_MAX  1024

void start_talking(int client_socket)
{
  /* Initialize the tx/rx buffers */
  char send_buf[SEND_MAX];
  char recv_buf[SEND_MAX];
  memset(recv_buf, 0, SEND_MAX);
  memset(send_buf, 0, SEND_MAX);

  int bytes;

  /* Setup input prompt */
  printf("FancyChat# ");

  fgets(send_buf, SEND_MAX-1, stdin);
  bytes = send(client_socket, send_buf, strlen(send_buf), 0);
  if(bytes > 0)
  {
    printf("[Message sent successfully!]\n");
  } else
  {
    perror("send() failed");
  }

  bytes = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
  if(bytes > 0)
  {
    printf(">>> %s\n",recv_buf);
  } else
  {
    perror("recv() failed");
  }
}

int main()
{

  struct sockaddr_in srv_sockaddr;
  struct hostent *server;
  int client_socket;

  server = gethostbyname("localhost");
  memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));
  srv_sockaddr.sin_family = AF_INET;
  memcpy((char *)server->h_addr,
         (char *)&srv_sockaddr.sin_addr.s_addr,
         server->h_length);
  srv_sockaddr.sin_port = htons(5432);

  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (connect(client_socket,
              (struct sockaddr *)&srv_sockaddr,
              sizeof(srv_sockaddr)) == -1)
  {
    perror("connect() failed");
    exit(1);
  }

  while (1)
  {
    start_talking(client_socket);
  }

  close(client_socket);
  return 0;
}
