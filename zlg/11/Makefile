#Makefile for file_rw
EXE = file_lseek
OBJ = $(EXE).o 
SRC = $(EXE).c

CC = gcc
CFLAGS = -g
# LDFLAGS = -l . -lF00

EXE:$(OBJ)
	$(CC) -o $(EXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $^

.PHONY:clean
clean:
	-$(RM) $(OBJ) $(EXE)
# 	-$(RM) hello.txt