CC = gcc
CFLAGS = -Wall -Werror

nyush: shell3.c
	$(CC) $(CFLAGS) -o nyush shell3.c

.PHONY: clean

clean:
	rm -f nyush

