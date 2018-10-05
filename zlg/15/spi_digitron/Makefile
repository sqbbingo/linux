
CFLAGS += -Wall
obj := spi_led_test
src := spi_led_test.c
CC  := arm-fsl-linux-gnueabi-gcc 

$(obj): $(src)
	$(CC) $(CFLAGS) $^ -o $@ -g

.PHONY: clean
clean:
	-rm spi_led_test
