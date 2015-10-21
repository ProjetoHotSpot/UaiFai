#include "routes.h"

static route route_array[MAX_ENTRY_ROUTE_TABLE];

static int route_to_fill = 0;

/* Registra uma nova rota, recebe o caminhho e a funcao */
void add_route(const char *path, int(*fn) (struct mg_connection *conn)) {
	strncpy(route_array[route_to_fill].prefix, path, LEN_PREFIX_ROUTE_TABLE);
	route_array[route_to_fill].fn = fn;
	route_to_fill++;
}

/* Inicia a tabela de rotas limpando seus valores */
void init_routes_table() {
	int i;
	for (i = 0; i < MAX_ENTRY_ROUTE_TABLE; i++) {
		strcpy(route_array[i].prefix, "");
		route_array[i].fn = NULL;
	}
}

int exec_route(const char *prefix, struct mg_connection *conn) {
	int i, rc;
	route aux;

	for (i = 0; aux.fn != NULL; i++) {
		aux = route_array[i];
		if ((rc = strcmp(aux.prefix, prefix)) == 0) {
			return aux.fn(conn);
		}
	}

	return 404;
}
