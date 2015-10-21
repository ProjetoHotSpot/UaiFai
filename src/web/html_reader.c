#include "html_reader.h"

int html_reader(char file_name[], char buffer[], int size_buffer, int initial_char_pos){
    char ch;
    int x;
    FILE *arq;

    arq = fopen(file_name, "r");

    if ( arq == NULL ) {
        printf("[Erro]: nao foi possivel abrir o arquivo %s\n", file_name);
        return 0;
    } else {
        // Lê o arquivo ate o limite do tamanho do buffer
        for ( x = initial_char_pos; (ch = fgetc(arq)) != EOF && x < (size_buffer-1); x++ ) {
            buffer[x] = ch;
        }
        // reserva o ultimo espaco do buffer para terminar a string
        buffer[x] = '\0';

        if(fclose(arq) == EOF){
            printf("[Erro]: nao foi possivel fechar o arquivo %s\n", file_name);
            return 0;
        }
    }

    // Retorna o indice do fim da string
    return x;
}
