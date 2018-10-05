#include <stdio.h>

void show(int a[],int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		printf("%d\t", a[i]);
	}
	printf("\n");
}

void revert(int a[],int len)
{
	if(len <= 1)
	{
		return ;
	}

	revert(a+1, len-2);
	
	int tmp;
	tmp = a[0];
	a[0] = a[len-1];
	a[len-1] = tmp;
}

int main()
{
	int a[] = {1,2,3,4,5,6,7};
	int len = sizeof(a)/sizeof(a[0]);
	int b = 2;
	printf("sizeof b = %d,sizeof(a) = %d,sizeof(a[0]) = %d\n",sizeof(b),sizeof(a),sizeof(a[0]));
	
	show(a,len);
	revert(a,len);
	show(a,len);
}
