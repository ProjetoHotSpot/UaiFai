

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/mongoose.h"
#include "../lib/sqlite3.h"

#define Tbuffer 150000

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
    char buffer[Tbuffer];
    buffer[0] = '\x0';
    FILE *arq;
    char ch;
    int x;

    switch (ev) {
        case MG_AUTH:
            return MG_MORE;

        case MG_REQUEST:
            mg_send_header(conn, "Content-Type", "text/html");
            
            char index[] ="web/index.html";
            arq = fopen(index, "r");
            if (arq == NULL) {
                printf("Error 404 - Página não encontrada\n");
            } else {
                for (x = 0; (ch = fgetc(arq)) != EOF && x < Tbuffer; x++) {
                    buffer[x] = ch;
                }
               fclose(arq);
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

    // Requisição do servidor. Ctrl-C Para terminar o programa
    printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Limpa e libera a instancia
    mg_destroy_server(&server);

    return 0;
}


