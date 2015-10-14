#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"

#define TAMBUFF 1500000
#define TAMBUFF_l 500000

void login (char *l, size_t s){
	
	char buffer_l[TAMBUFF_l];
    buffer_l[0] = '\x0';
	
	
	
}

int home(char *b, size_t s){

    char buffer[TAMBUFF];
    buffer[0] = '\x0';

strncpy(buffer,
	"<!DOCTYPE: text/html>\n\n" 
	"<html><head>\n" 
	"<meta charset='UTF-8'>\n"
	"<title>Danake</title>\n" 
		// 1 - Css
	"</head>\n"	
	"<body>\n" 
	"<h1>OlÃ¡, Mundo!</h1>\n" 
		login();
	"</body>\n" 
	"</html>",
	sizeof(buffer));

strncat(b, buffer, s); 
return 1;
}
