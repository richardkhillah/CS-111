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

build: lab2_add mks #lab2_list

lab2_add:
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c add_member.c common.c 

lab2_list:
	$(CC) $(CFLAGS) -o

mks:
	touch l2add_tests.mk l2list_tests.mk
	chmod +x generate.sh
	./generate.sh

-include l2add_tests.mk

# Rules declared in add mk file
#ADDRULE="test_add"
#LISTRULE="test_list"

tests: clean_slate tests_add

##
# Used to ensure fresh .mk and .csv files
clean_slate: mks 
tests_add: test_add

GSCRIPTS=lab2_add.gp lab2_list.gp

##
# TODO: possibly remove dependency on tests to allow user to use previously
# generated numbers
graphs: tests
	./lab2_add.gp

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${SOURCE} ${GSCRIPTS}

clean:
	rm -rf lab2_list lab2_add *tar.gz *.mk *.csv