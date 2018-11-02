# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

CC=gcc
CFLAGS= -Wall -Wextra -pthread -std=gnu99

DEFAULT=Makefile README
COMMON=common.c common.h

ADD_SOURCE=lab2_add.c add_member.h add_member.c
LIST_SOURCE=lab2_list.c SortedList.h SortedList.c list_member.h list_member.c

OTHER_FILES= *.png *.csv

#default: build

build: lab2_add lab2_list mks

lab2_add:
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c add_member.c common.c 

lab2_list:
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c list_member.c common.c

mks:
	touch maketests.mk
	chmod +x generate.sh
	./generate.sh

-include maketests.mk

tests: test_add test_list

# Used to ensure fresh .mk and .csv files
# clean_slate: mks 
# tests_add: test_add 
# tests_list: test_list

GSCRIPTS=lab2_add.gp lab2_list.gp

graphs:
	./lab2_add.gp
	./lab2_list.gp

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${ADD_SOURCE} ${LIST_SOURCE} ${GSCRIPTS} ${OTHER_FILES}
clean:
	rm -rf lab2_list lab2_add *tar.gz *.mk *.csv *.png
