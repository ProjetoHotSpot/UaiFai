

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/mongoose.h"
#include "../lib/sqlite3.h"
#include "../lib/index.h"

#define Tbuffer 150000


static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
    char buffer[Tbuffer];
    buffer[0] = '\x0';

    switch (ev) {
        case MG_AUTH:
            return MG_MORE;

        case MG_REQUEST:
	
			mg_send_header(conn, "Content-Type", "text/html");
		
			if(home(buffer, sizeof(buffer)) == 0){
				return MG_FALSE;
			}
		
			mg_printf_data(conn, buffer);
			return MG_TRUE;	
			
        default:
            return MG_FALSE;
    }
}


int main(void) {

    struct mg_server *server;
    // Criando e configurando o servidor
    server = mg_create_server(NULL, ev_handler);
    mg_set_option(server, "document_root", ".");
    mg_set_option(server, "listening_port", "8080");

    // requisiçao do servidor. Ctrl-C Para terminar o programa
    printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Limpa e libera a instancia
    mg_destroy_server(&server);

    return 0;
}
