#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json.h>

#include "server.h"

int main(void) {
    const char json_str[100] = "{\"name\":\"john doe\"}";
    struct json_object *json = json_tokener_parse(json_str);
    if(json == NULL){
        puts("error: json can not parse");
        return EXIT_FAILURE;
    }
    printf("%s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

    config_t cfg;
    cfg.domain = AF_INET;
    cfg.port = 8080;
    cfg.service = SOCK_STREAM;
    cfg.protocol = 0; 
    cfg.backlog = 10;
    cfg.interface = INADDR_ANY;
    
    server_t server;
    
    new_server(&server, &cfg);

    server_start(&server);

    return EXIT_SUCCESS;
}

