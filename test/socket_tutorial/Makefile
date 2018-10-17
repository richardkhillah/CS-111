CC=gcc
CFLAGS= -Wall -Wextra

SOCFLAGS=

default: server client

server: server.c
	$(CC) $(CFLAGS) -o $@ $< 

client: client.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -rf server client
