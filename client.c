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

#define BUF_SIZE 1024

void send_message(int client_socket)
{
  char send_buf[BUF_SIZE];
  memset(send_buf, 0, BUF_SIZE);
  int bytes;

  fgets(send_buf, BUF_SIZE-1, stdin);
  bytes = send(client_socket, send_buf, BUF_SIZE, 0);
  if(bytes < 0)
  {
    perror("send() failed");
  }
}

void read_message(int client_socket)
{
  char recv_buf[BUF_SIZE];
  memset(recv_buf, 0, BUF_SIZE);
  int bytes;

  bytes = recv(client_socket, recv_buf, BUF_SIZE, 0);
  if(bytes > 0)
  {
    printf("%s\n",recv_buf);
  } else
  {
    perror("recv() failed");
  }
}

int main()
{
  struct sockaddr_in srv_sockaddr;
  struct hostent *server;
  int act, max_fd, client_socket;
  fd_set write_fd_set, read_fd_set, master;

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
  FD_ZERO(&read_fd_set);
  FD_ZERO(&write_fd_set);
  FD_ZERO(&master);
  FD_SET(client_socket, &master);
  max_fd = client_socket;

  while (1)
  {
    read_fd_set = master;
    write_fd_set = master;
    act = select(max_fd + 1, &read_fd_set, &write_fd_set, NULL, NULL);
    if (act < 0)
    {
      perror("select() failed");
      exit(1);
    } else
    {
      printf("Select for %d sockets\n", act);
    }

    if(FD_ISSET(client_socket, &read_fd_set))
    {
      printf("Read.\n");
      read_message(client_socket);
    }

    else if(FD_ISSET(client_socket, &write_fd_set))
    {
      printf("Write.\n");
      send_message(client_socket);
    };
  }

  close(client_socket);
  return 0;
}
