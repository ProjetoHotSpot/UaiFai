#include "bancodedados.h"
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int conectaBD(int argc, char* argv[]){
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("bancodedados.db", &db);
   if( rc ){
      fprintf(stderr, "Erro ao conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Banco de Dados conectado com sucesso!\n");
   }
}

void encerraBD() {
    sqlite3_free(MsgErro);
	sqlite3_close(db);
}

int criaTabela(){
	
   /* Create SQL statement */
   sql = "CREATE TABLE IF NOT EXISTS cliente("  \
         "id			 INT PRIMARY KEY NOT NULL AUTOINCREMENT," \
         "usuario        TEXT    NOT NULL," \
         "nome           TEXT    NOT NULL," \
         "aniversario    TEXT    NOT NULL," \
         "sexo			 TEXT    NOT NULL," \
		 "link           TEXT    NOT NULL," \
		 "email          TEXT    NOT NULL," \
		 "localizacao    TEXT    NOT NULL) ;";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "Erro ao criar tabela %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Tabela criada com sucesso!\n");
   }
   encerraBD();
   return 0;
}

int insereCliente(cliente c) {

    int conecta = conectaBD();

	sql = "INSERT INTO usuario(usuario, aniversario, link, nome, localizacao, sexo, email) VALUES(?, ?, ?, ?, ?, ?, ?, ?);";

    if (conecta != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        encerraBD();
        return 0;

        } else{

        msg = sqlite3_exec(db, sql, callback, 0, &MsgErro);
        if (msg != SQLITE_OK ) {

            fprintf(stderr, "Erro ao inserir cliente: %s\n", MsgErro);
            encerraBD();
            return 0;
        } else {
                  sqlite3_bind_null(stmt, 1);
                  sqlite3_bind_text(stmt, 2, c.usuario, strlen(c.usuario), 0);
                  sqlite3_bind_text(stmt, 3, c.aniversario, strlen(c.aniversario), 0);
                  sqlite3_bind_text(stmt, 4, c.link, strlen(c.link), 0);
                  sqlite3_bind_text(stmt, 5, c.nome, strlen(c.nome), 0);
                  sqlite3_bind_text(stmt, 6, c.localizacao, strlen(c.localizacao), 0);
                  sqlite3_bind_text(stmt, 7, c.sexo, strlen(c.sexo), 0);
                  sqlite3_bind_text(stmt, 8, c.email, strlen(c.email), 0);
        }

        // commit
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        encerraBD();
        return 1;
    }
}

int atualizaCliente(cliente c) {
    sql = "UPDATE usuario SET aniversario = ?, link = ?, nome = ?, localizacao = ?, sexo = ?, email = ? WHERE usuario = ?;";

    int conecta = conectaBD();
    if (conecta != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        encerraBD();

        return 0;

        }
	else{

        msg = sqlite3_exec(db, sql, callback, 0, &MsgErro);
        if (msg != SQLITE_OK ) {
            fprintf(stderr, "Erro ao atualizar cliente: %s\n", MsgErro);
            encerraBD();

            return 0;
        }
		else {
            sqlite3_bind_text(stmt, 1, c.aniversario, strlen(c.aniversario), 0);
            sqlite3_bind_text(stmt, 2, c.link, strlen(c.link), 0);
            sqlite3_bind_text(stmt, 3, c.nome, strlen(c.nome), 0);
            sqlite3_bind_text(stmt, 4, c.localizacao, strlen(c.localizacao), 0);
            sqlite3_bind_text(stmt, 5, c.sexo, strlen(c.sexo), 0);
            sqlite3_bind_text(stmt, 6, c.email, strlen(c.email), 0);
            sqlite3_bind_text(stmt, 7, c.usuario, strlen(c.usuario), 0);
        }

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        encerraBD();
        return 1;
    }
}

int deletaCliente(cliente c) {

	sql = "DELETE FROM usuario WHERE usuario = ?;";

    int conecta = conectaBD();
    if (conecta != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        encerraBD();

        return 0;

    } 
	else {
        msg = sqlite3_prepare(db, sql, -1, &stmt, 0);

        if (msg != SQLITE_OK ) {
            fprintf(stderr, "Erro ao deletar cliente: %s\n", MsgErro);
            encerraBD();

            return 0;

        } else {
            sqlite3_bind_text(stmt, 1,  c.cliente, strlen(c.cliente), 0);
        }

        encerraBD();
    }
    return 1;
}

int allClientes() {
    sql = "SELECT * FROM cliente;";
    int conecta = conectaBD();
	if (conecta != SQLITE_OK) {
        
        fprintf(stderr, "Erro ao conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        encerraBD();
		
        return 0;

    	}
	else{
        msg = sqlite3_exec(db, sql, callback, 0, &MsgErro);

		if (msg != SQLITE_OK ) {
		
            fprintf(stderr, "Erro ao realizar consulta ao banco de dados\n");
            fprintf(stderr, "Erro consultar cliente: %s\n", MsgErro);
			sqlite3_free(MsgErro);
            encerraBD();
		
            return 0;
        }
		
    	encerraBD();
        return 1;
	}
}
