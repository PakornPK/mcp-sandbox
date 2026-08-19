#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_BUFFER 16000

typedef struct config {
    int domain;
    int port;
    int service;
    int protocol;
    int backlog;
    unsigned long interface;
} config_t;

typedef struct server {
    int socket;
    struct sockaddr_in address;
    config_t config;
} server_t;

void new_server(server_t *self, config_t *cfg);

void server_start(server_t *self);

#endif
