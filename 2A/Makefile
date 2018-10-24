# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

CC=gcc
CFLAGS= -Wall -Wextra

DEFAULT=Makefile README
HEADERS= common.h SortedList.h
SOURCE= SortedList.c

default: lab2_list

lab2_list: SortedList.c ${HEADERS}
	$(CC) $(CFLAGS) -o $@ $<

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${SOURCE} ${HEADERS}
clean:
	rm -rf lab2_list *tar.gz