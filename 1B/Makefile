# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# UID: 604853262

CC=gcc
CFLAGS= -Wall -Wextra

all: lab1b-client lab1b-server

lab1b-client: lab1b-client.c log.h
	$(CC) $(CFLAGS) -o $@ $<

lab1b-server: lab1b-server.c
	$(CC) $(CFLAGS) -o $@ $<

dist: tar -czvf lab1b-604853262.tar.gz README Makefile lab1b-client.c lab1b-server.c my.key

clean:
	rm -rf lab1b-client lab1b-server *tar.gz 
