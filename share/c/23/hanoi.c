#include <stdio.h>

void hanoi(int n,char one,char two,char three)
{
	void move(char x,char y);
	if(n == 1)
	{
		move(one ,three);
	}
	else
	{
		hanoi(n-1,one,three,two);
		move(one,three);
		hanoi(n-1,two,one,three);
	}
}

void move(char x,char y)
{
	printf("%c-->%c\n",x,y );
}

int main(int argc, char const *argv[])
{
	void hanoi(int a,char one,char two,char three);
	int m;
	printf("input the number of diskes");
	scanf("%d",&m);
	printf("The step to move %d diskes:\n",m );
	hanoi(m,'A','B','C');

	return 0;
}