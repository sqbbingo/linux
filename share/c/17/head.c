#include <stdio.h>

int main(int argc, char const *argv[])
{
	char *p = (char *)malloc(100);
	bzero(p,100);

	int *k = calloc(25,sizeof(int));

	p = realloc(p,200);

	free(p);
	free(k);
	
	return 0;
}