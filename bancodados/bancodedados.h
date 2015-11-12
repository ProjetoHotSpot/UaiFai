#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../lib/mongoose.h"
#include "../lib/sqlite3.h"
#include "../src/routes.h"
#include "../src/resources.h"

#ifndef __DB__
#define __DB__

	typedef struct{
		int id;
		char usuario[40];
		char aniversario[15];
		char link[100];
		char nome[100];
		char localizacao[100];
		char sexo[10];
		char email[80]; 
	} cliente;

	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	int conectaBD();
	int criaTabela();
	int insereCliente(cliente c);
	int atualizaCliente(cliente c);
	int deletaCliente(cliente c);
	int allClientes();
	void encerraBD();

#endif
