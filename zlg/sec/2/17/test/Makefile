EXEC	=./char_test
OBJS	=char_test.o 

CROSS	=arm-fsl-linux-gnueabi-
CC	=$(CROSS)gcc
STRIP	=$(CROSS)strip 
CFLAGS	=-Wall -g -o2

all:	clean $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	$(STRIP) $@
clean:
	-rm -f $(EXEC) *.o
