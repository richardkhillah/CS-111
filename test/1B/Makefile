
CC=gcc
CFLAGS= -Wall -Wextra

default: lab1b-client

objs=lab1b-client.o common.o

lab1b-client: $(objs)
	$(CC) $(CFLAGS) -o $@ $(objs)

lab1b-client.o common.o: common.h

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm -rf lab1b-client *.o *.so
