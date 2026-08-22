#ifndef POSTGRES_H
#define POSTGRES_H

#include <libpq-fe.h>

typedef enum {
    PG_OK = 0,
    PG_ERR_ALLOC,
    PG_ERR_CONNECT,
    PG_ERR_INVALID_ARG,
} pgstatus_t;

typedef struct pgctx_t {
   PGconn *conn;
   PGresult *result;
} pgctx_t;

typedef struct pgconfig_t {
    char *host;
    int port;
    char *db_name;
    char *username;
    char *password;
} pgconfig_t;

pgstatus_t connect_database(pgctx_t **ctx, pgconfig_t *config);  
void  disconnect_database(pgctx_t **ctx);

#endif
