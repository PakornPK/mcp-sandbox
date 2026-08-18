#include <stdio.h>
#include <stdlib.h>
#include <json.h>

int main(void) {
    const char json_str[100] = "{\"name\":\"john doe\"}";
    struct json_object *json = json_tokener_parse(json_str);
    if(json == NULL){
        puts("error: json can not parse");
        return EXIT_FAILURE;
    }
    printf("%s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
    return EXIT_SUCCESS;
}

