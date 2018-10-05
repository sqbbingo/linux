#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	char s1[10] = {'a','b','c','d','\0'};
	char s2[5] = {"ABCD"};
	char s3[5] = "abc";
	char a[7] = "abcdef";
	char b[5] = "ABCD";

	strcpy(a,b);
	printf("a = %s\n", a);

	printf("s3[1] = %c\n",s3[1] );
	printf("abc[1] = %c\n","abc"[1] );

	s3[0] = 'A';
	s3[1] = 'B';
	s3[2] = 'C';

	printf("s2.len = %d\n",strlen(s2));
	printf("s.sizeof = %d\n",sizeof(s2));

	strncpy(s1,s2,2);
	printf("s1 = %s\n",s1 );
	strcpy(s1,s2);
	printf("s1n = %s\n",s1 );

	strcat(s1,s2);
	printf("s1c = %s\n",s1 );
	strncat(s1,s2,2);
	printf("s1cn = %s\n",s1 );

	return 0;
}
