EXEC	= test_uart_1
OBJS    = test_uart.o

CROSS = 
CC	= $(CROSS)gcc 

STRIP	= $(CROSS)strip
CFLAGS	= -g -O2

all:  clean $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(CFLAGS)	-o $@ $(OBJS)

clean:
	-rm -f $(EXEC) *.o

