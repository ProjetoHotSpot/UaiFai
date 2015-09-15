#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
int i,j;
register int m,n;
double t;

t=clock();
for (j=0;j<50000;j++)
	for (i=0;i<50000;i++);	
t = clock()-t;
printf("Tempo dos laços nao register: %1f\n", t);

t=clock();
for (m=0;m<50000;m++)
	for (n=0;n<50000;n++);
t=clock()-t;
printf("Tempo dos laços register: %1f\n", t);

system("PAUSE");
return 0;

}

