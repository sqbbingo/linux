#!/bin/sh

echo "hd-new doesn't exist"
ls -l
cat > hd-new.c <<EOF
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("hello here documents\n");

	return EXIT_SUCCESS;
}
EOF
cc -W -Wall -o hd-new hd-new.c
ls -l
./hd-new
rm hd-new hd-new.c