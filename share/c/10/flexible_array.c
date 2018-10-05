#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node
{
	int a;
	float b;
	char c;

	char msg[0];
};

int main(int argc, char const *argv[])
{
	/* code */
	char buf[100] = {'a','y','u',[90 ... 95]='w'};
	fgets(buf,100,stdin);

	int n = strlen(buf)+1;

	struct node *p = malloc(sizeof(struct node)+n);
	
	p->a = 100;
	p->b = 3.14;
	p->c = 'w';
	strncpy(p->msg,buf,n);

	printf("sizeo of struct:%d\n", sizeof(*p)+n );
	printf("msg: %s\n",p->msg );

	return 0;
}