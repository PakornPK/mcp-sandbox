#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
#include "picohttpparser.h"

void new_server(server_t *self, config_t *cfg) {
    struct sockaddr_in addr;
    
    memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = cfg->domain;
    addr.sin_port = htons(cfg->port);
    addr.sin_addr.s_addr = htonl(cfg->interface);
    
    int _socket = socket(cfg->domain, cfg->service, cfg->protocol);
    if(_socket < 0) {
        fprintf(stderr, "error: socket create failed\n");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        fprintf(stderr, "error: set socket otp failed\n");
        exit(EXIT_FAILURE);
    }

    int _bind = bind(_socket, (struct sockaddr *)&addr, sizeof(addr));
    if(_bind < 0) {
        fprintf(stderr, "error: bind failed\n");
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
    const char *method = NULL;
    size_t method_len = 0;
    const char *path = NULL;
    size_t path_len = 0;
    int minor_version = 0;
    struct phr_header headers[100];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);
    char buffer[DEFAULT_BUFFER];
    print_sockaddr((struct sockaddr *)&self->address);
    while (1) {
        printf("=== WAITING FOR CONNECTION === \n");
        int addrlen = sizeof(self->address);
        int new_socket = accept(self->socket, (struct sockaddr*)&self->address, (socklen_t*)&addrlen);
        ssize_t bytesRead = read(new_socket, buffer, DEFAULT_BUFFER - 1);
        if (bytesRead >= 0) {
            buffer[bytesRead] = '\0';
            int bytes_parsed = phr_parse_request(buffer, strlen(buffer), 
                    &method, &method_len, 
                    &path, &path_len, 
                    &minor_version,
                    headers, &num_headers, 0);
            if (bytes_parsed == -1) {
               printf("Error: Request parsing failed (malformed format).\n");
            } else if (bytes_parsed == -2) {
               printf("Status: Request is incomplete (awaiting more data over socket).\n");
            }
            printf("--- Request Line ---\n");
            printf("Method:  %.*s\n", (int)method_len, method);
            printf("--- Request Line ---\n");
            printf("Method:  %.*s\n", (int)method_len, method);
            printf("Path:    %.*s\n", (int)path_len, path);
            printf("Version: HTTP/1.%d\n\n", minor_version);

            printf("--- Headers (%zu found) ---\n", num_headers);
            for (size_t i = 0; i < num_headers; i++) {
                printf("%.*s: %.*s\n", 
                    (int)headers[i].name_len, headers[i].name, 
                    (int)headers[i].value_len, headers[i].value
                );
            }
            const char *body = buffer + bytes_parsed;
            printf("\n--- Body ---\n");
            printf("%s\n", body);
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
