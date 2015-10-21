#ifndef __ROUTES__
#define __ROUTES__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../3rdparty/mongoose.h"

#define MAX_ENTRY_ROUTE_TABLE 10000
#define LEN_PREFIX_ROUTE_TABLE 1024

typedef struct route_t {
	char prefix[LEN_PREFIX_ROUTE_TABLE];
	int(*fn) (struct mg_connection *conn);
} route;

/* Registra uma nova rota, recebe o caminhho e a funcao */
void add_route(const char *prefix, int(*fn) (struct mg_connection *conn));

/* Inicia a tabela de rotas limpando seus valores */
void init_routes_table();

/* Entrega a rota para o prefix pesquisado */
int exec_route(const char *prefix, struct mg_connection *conn);

#endif
