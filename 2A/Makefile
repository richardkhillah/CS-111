# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

CC=gcc
CFLAGS= -Wall -Wextra

DEFAULT=Makefile README
COMMON=common.c

ADD_SOURCE=lab2_add.c add-member.c

OTHER= *.png *.csv

default: build

build: lab2_add #lab2_list

lab2_add: lab2_add.c add-member.c common.c #${ADD_SOURCE}
	$(CC) $(CFLAGS) -o $@ $<

lab2_list: ${LIST_SOURCE}  #${HEADERS}
	$(CC) $(CFLAGS) -o $@ $<

tests:

graphs:

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${SOURCE}

clean:
	rm -rf lab2_list *tar.gz