# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

CC=gcc
CFLAGS= -Wall -Wextra -pthread

DEFAULT=Makefile README
COMMON=common.c common.h

ADD_SOURCE=lab2_add.c add_member.c add_member.h

OTHER= *.png *.csv

#default: build

build: lab2_add #lab2_list

lab2_add:
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c add_member.c common.c 

lab2_list:
	$(CC) $(CFLAGS) -o

tests:

graphs:

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${SOURCE}

clean:
	rm -rf lab2_list lab2_add *tar.gz