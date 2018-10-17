# this is a test makefile used to build common utilities
# General Notes:
# 1. in myprog compile line (i.e. the line that has
#    $(CC) in it), $(objs) MUST be used insead of using $<. I do not
#    know why, however I "think" it has to do with parameter expansion.
# 2. the %.o: %.c thing works exactly as it should. do not change.


CC=gcc
CFLAGS= -Wall -Wextra
LDFLAGS= -ldl -Wl,-rpath,$(PWD)

#MOREFLAGS= -march=native -mtune=native #-mrdrnd

objs= myprog.o common.o

default: myprog commonrw-server.so commonrw-client.so

# $< does not serve as a replacement for $(objs) in compile line
myprog: $(objs)
	$(CC) $(CFLAGS) -o $@ $(objs) $(LDFLAGS)

myprog.o common.o: common.h

commonrw-server.o commonrw-client.o: common.h

commonrw-server.so: commonrw-server.o
	$(CC) $(CFLAGS) -shared -o $@ commonrw-server.o

commonrw-client.so: commonrw-client.o
	$(CC) $(CFLAGS) -shared -o $@ commonrw-client.o

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm *.o *.so myprog
