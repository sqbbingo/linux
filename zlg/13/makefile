#!/bin/bash

all:pthread_join pthrea_create_exit

pthread_join:pthread_join.c
	gcc pthread_join.c -o pthread_join -pthread -lm

pthrea_create_exit:pthrea_create_exit.c
	gcc pthrea_create_exit.c -o pthrea_create_exit -pthread

.PHONLY:clean
clean:
	-rm pthrea_create_exit pthread_join
