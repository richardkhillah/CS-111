# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# UID: 604853262

CC=gcc
CFLAGS= -Wall -Wextra
DEBUG= --degbug

all: lab1a

lab1a: lab1a.c
	$(CC) $(CFLAGS) -o $@ $<

dist:
	tar -cvzf lab1a-604853262.tar.gz lab1a.c Makefile README

clean: 
	rm -rf lab1a *tar.gz
