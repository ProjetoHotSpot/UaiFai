#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/mongoose.h"
#include "../libs/sqlite3.h"

#define OUTPUT_BUFFER_SIZE 150000

static int ev_handler(struct mg_connection *conn, 
                      enum mg_event ev) {
  char buffer[OUTPUT_BUFFER_SIZE];
  char index[] = "htdocs/index.html";
  char ch;
  int x;
  FILE *arq;
  buffer[0] = '\x0';

  switch (ev) {
    case MG_AUTH:
		return MG_MORE;
    case MG_REQUEST:
		//REQUISIÇÕES DE PÁGINA
		
		mg_send_header(conn, "Content-Type", "text/html");
		
		arq = fopen(index, "r");
		
		if ( arq == NULL ) {
			printf("[Erro]: nao foi possivel abrir o arquivo htdocs/index.html\n");
		} else {
			for ( x = 0; (ch = fgetc(arq)) != EOF && x < OUTPUT_BUFFER_SIZE ; x++ ) {
				buffer[x] = ch;
			}

			if(fclose(arq) == EOF){
				printf("[Erro]: nao foi possivel fechar o arquivo htdocs/index.html\n");
			}
		}	
		
		mg_printf_data(conn, buffer);
		
		return MG_TRUE;
    default: 
		return MG_FALSE;
  }
}

int main(void){
	struct mg_server *server;

	// Create and configure the server
	server = mg_create_server(NULL, ev_handler);
	mg_set_option(server, "document_root", ".");
	mg_set_option(server, "listening_port", "8080");

	// Serve request. Hit Ctrl-C to terminate the program
	printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
	for (;;) {
		mg_poll_server(server, 1000);
	}

	// Cleanup, and free server instance
	mg_destroy_server(&server);

	return 0;
}
