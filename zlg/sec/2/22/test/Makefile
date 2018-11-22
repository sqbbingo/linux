EXE	=./char_dev_rw_test
OBJ	=char_dev_rw_test.o
SRC	=char_dev_rw_test.c

CROSS	=arm-fsl-linux-gnueabi-
CC	=$(CROSS)gcc
STRIP	=$(CROSS)strip
CFLAGS	=-Wall -g -o2

all:	clean $(EXE)
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	$(STRIP) $@
clean:
	-rm -f $(EXE) *.o