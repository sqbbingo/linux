#进程基本操作文件process_base.c
EXE = process_base
OBJ = $(EXE).o
SRC = $(EXE).c

CC = gcc
CFLAGS = -g

EXE:$(OBJ)
	$(CC) -o $(EXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $^

.PHONY:clean
clean:
	-$(RM) $(OBJ) $(EXE)