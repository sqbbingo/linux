#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int a = 100;
	int *p = &a;
	printf("*p = %d\n", *p);

    int b = 200;
    p = &b;
    printf("*p = %d\n",*p );

    *p = 201;

    int *(*k);
    k = &p;

    **k = 202;

    printf("*p = %d\n", *p );
    printf("b = %d\n", b );

    int number[4] = {11,22,33,44};
    p = number;
    p = p+2;
    printf("p+2 = %d\n",*p );

    int *p1;
    float *p2;
    struct node *p4;

    void *p5;
    p5 = malloc(100);

    int i;
    for(i=0; i<25; i++)
    {
    	*((int *)p5 + i) = i+1;
    }

    for (int i = 0; i < 25; i++)
    {
    	printf("p5(%d) = %d\n",i,*((int *)p5+i) );
    }

    double *p6 = malloc(20*sizeof(double));
   // double *p6 = (double *)malloc(20*sizeof(double));

    for (int i = 0; i < 20; i++)
    {
    	*(p6+i) = 100.0;
    	printf("p6(%d) = %f\n", i, *(p6+i) );
    }

	return 0;
}