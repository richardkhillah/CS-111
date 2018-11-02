# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262

CC=gcc
CFLAGS= -Wall -Wextra -pthread -std=gnu99

DEFAULT=Makefile README
COMMON=common.c common.h

ADD_SOURCE=lab2_add.c add_member.h add_member.c
LIST_SOURCE=lab2_list.c SortedList.h SortedList.c list_member.h list_member.c

OTHER_FILES= *.png *.csv

#default: build

build: lab2_add lab2_list

lab2_add:
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c add_member.c common.c 

lab2_list:
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c list_member.c common.c

mks:
	touch maketests.mk
	./generate.sh

-include maketests.mk

tests: test_add test_list

# Used to ensure fresh .mk and .csv files
# clean_slate: mks 
# tests_add: test_add 
# tests_list: test_list

GSCRIPTS=lab2_add.gp lab2_list.gp

graphs: tests
	./lab2_add.gp
	./lab2_list.gp
IMAGES=lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png
CSVS=lab2_add.csv lab2_list.csv

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${ADD_SOURCE} ${LIST_SOURCE} ${GSCRIPTS} ${COMMON} ${IMAGES} ${CSVS}

clean:
	rm -rf lab2_list lab2_add *tar.gz *.png *.csv

check:
	./P2A_check.sh ${UID}

test_add: lab2_add
	-./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv

	-./lab2_add --threads=1 --iterations=10 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=20 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=40 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=80 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=20 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=40 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=80 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=20 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=40 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=80 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=20 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=40 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=80 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=20 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=40 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=80 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=20 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=40 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=80 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=20 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=40 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=80 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=100 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=1000 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=20 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=40 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=80 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=20 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=40 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=80 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100000 --yield >> lab2_add.csv

	-./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=100000 --yield >> lab2_add.csv

	-./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv

	-./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=1000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=1000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=m --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=c --yield >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=1000 --sync=s --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=m --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=c --yield >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=1000 --sync=s --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=m --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=c --yield >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=1000 --sync=s --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=m --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=c --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000 --sync=s --yield >> lab2_add.csv

	-./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv


test_list: lab2_list
	-./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
	-./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv


	-./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

	-./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

