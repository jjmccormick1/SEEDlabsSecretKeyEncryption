CC=gcc
CFLAGS=  -Og -ggdb

all:
	@$(CC) $(CFLAGS) getkey.c -o getkey -lcrypto -ldl
run: all
	@./getkey words.txt
clean:
	@rm -f  getkey

