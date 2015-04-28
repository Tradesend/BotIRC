/*
** bot.c for bot in /home/moran-_d/rendu/pathwar
**
** Made by moran-_d
** Login   <moran-_d@epitech.net>
**
** Started on  Mon Apr 27 09:25:45 2015 moran-_d
** Last update Tue Apr 28 14:01:36 2015 Even David
*/

#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#define MAXLINE 4096

int my_connect(char *server, char *port, int *sockfd);
int my_send(int sockfd, int debug, char *fmt, ...);
int my_read(int sockfd, char *recvline, int debug);

int main(int argc, char **argv) {
  int sockfd, n, debug;
  char recvline[MAXLINE+1];
  char *cmd, *pos, *tm;
  char server[] = "irc.pathwar.net";
  char user[] = "ouiti_f";
  char port[] = "6667";
  char ignore[] = "#pathwar-fr :";
  int ignoresize = sizeof(ignore) - 1;
  int joined = 0;
  char	*str;

  debug = 0;

  if (!my_connect(server, port, &sockfd))
    {
      printf("Failed to connect to %s.\n", server);
      return (0);
    }

  my_send(sockfd, debug, "NICK %s\r\n", user);
  sleep(1);
  my_send(sockfd, debug, "USER %s 0 0 :%s \r\n", user, user);
  sleep(2);
  my_send(sockfd, debug, "JOIN #pathwar-fr\r\n");

  while(1)
    {
      n = my_read(sockfd, recvline, debug);
      if (n > 0)
	{
	  recvline[n] = 0;
	  pos = recvline;
	  printf("IN: %s <<<\n", pos);
	  str = malloc(strlen("./script.sh ") + strlen(pos) + 1);
	  strcpy(str, "./script.sh ");
	  strcat(str, pos);
	  system(str);
	  free(str);
	  if ((tm = strstr(pos, "PING")) != NULL)
	    {
	      tm = strstr(tm, ":") + 1;
	      my_send(sockfd, debug, "PONG %s\r\n", tm);
	    }
	}
      if (++joined <= 2)
	my_send(sockfd, debug, "JOIN #pathwar-fr\r\n");
    }
  exit(0);
}

int my_connect(char *server, char *port, int *sockfd) {
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(server, port, &hints, &res);

  if ( (*sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    return 0;
  }
  if (connect(*sockfd, res->ai_addr, res->ai_addrlen) < 0 ) {
    return 0;
  }

  return 1;
}

int my_send(int sockfd, int debug, char *fmt, ...) {
  char buf[MAXLINE];
  va_list ap;

  memset(buf, 0, MAXLINE);
  va_start(ap, fmt);
  vsnprintf(buf, MAXLINE, fmt, ap);
  va_end(ap);
  printf("OUT: %s <<<\n", buf);
  return send(sockfd, buf, strlen(buf), 0);
}

int my_read(int sockfd, char *recvline, int debug) {
  int n;
  n = read(sockfd, recvline, MAXLINE);
  if (n > 0 && debug) {
    printf("IN: %s <<<\n", recvline);
  }
  return n;
}
