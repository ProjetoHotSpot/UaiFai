#include <stdio.h>
#include <stdlib.h>

int i ;
int j=234;

void func(void);
	int main(){
printf("%d\t%d\n", i, j);
func();
printf("%d\t%d\n", i, j);
system("PAUSE");
return 0;
}

void func(){
	i=25;
	j=48;
}
