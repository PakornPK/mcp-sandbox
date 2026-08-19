#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

void new_server(server_t *self, config_t *cfg) {
    struct sockaddr_in addr;
    
    memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = cfg->domain;
    addr.sin_port = htons(cfg->port);
    addr.sin_addr.s_addr = htonl(cfg->interface);
    
    int _socket = socket(cfg->domain, cfg->service, cfg->protocol);
    if(_socket < 0) {
        fprintf(stderr, "error: new_socket: socket create failed\n");
        exit(EXIT_FAILURE);
    }

    int _bind = bind(_socket, (struct sockaddr *)&addr, sizeof(addr));
    if(_bind < 0) {
        fprintf(stderr, "error: new_socket: bind failed\n");
        exit(EXIT_FAILURE);
    }

    int _listen = listen(_socket, cfg->backlog);
    if(_listen < 0) {
        fprintf(stderr, "error: new_socket: listen failed\n");
        exit(EXIT_FAILURE);
    }
    
    self->socket = _socket;
    self->address = addr;
    self->config = *cfg;
    puts("init server success...");
}

void print_sockaddr(const struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        struct sockaddr_in *sin = (struct sockaddr_in *)sa;
        char ip_str[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &(sin->sin_addr), ip_str, sizeof(ip_str));

        int port = ntohs(sin->sin_port);

        printf("IPv4 Address: %s:%d\n", ip_str, port);
    }
    else if (sa->sa_family == AF_INET6) {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
        char ip_str[INET6_ADDRSTRLEN];

        inet_ntop(AF_INET6, &(sin6->sin6_addr), ip_str, sizeof(ip_str));
        int port = ntohs(sin6->sin6_port);

        printf("IPv6 Address: [%s]:%d\n", ip_str, port);
    }
    else {
        printf("Unknown address family: %d\n", sa->sa_family);
    }
}

void server_start(server_t *self) {
    char buffer[DEFAULT_BUFFER];
    print_sockaddr((struct sockaddr *)&self->address);
    while (1) {
        printf("=== WAITING FOR CONNECTION === \n");
        int addrlen = sizeof(self->address);
        int new_socket = accept(self->socket, (struct sockaddr*)&self->address, (socklen_t*)&addrlen);
        ssize_t bytesRead = read(new_socket, buffer, DEFAULT_BUFFER - 1);
        if (bytesRead >= 0) {
            buffer[bytesRead] = '\0';
            puts(buffer);
        } else {
            perror("Error reading buffer...\n");
        }
        char *response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                         "<!DOCTYPE html>\r\n"
                         "<html>\r\n"
                         "<head>\r\n"
                         "<title>Testing Basic HTTP-SERVER</title>\r\n"
                         "</head>\r\n"
                         "<body>\r\n"
                         "Hello, World!\r\n"
                         "</body>\r\n"
                         "</html>\r\n";
        write(new_socket, response, strlen(response));
        close(new_socket);
    }
}
