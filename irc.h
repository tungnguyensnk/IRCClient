#ifndef IRCCLIENT_IRC_H
#define IRCCLIENT_IRC_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <malloc.h>

#define PORT 15000
#define HOST "127.0.0.1"
#define BUF_SIZE 2048
#define TIME_OUT 3

int connect_tcp(char *ip, int port);

char *getCodeResponse(char *data);

char *getNick(char *data);

char *recv_data(int socket);

void ping_pong(int sfd, char *data);

#endif //IRCCLIENT_IRC_H
