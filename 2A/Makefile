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
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c list_member.c common.c

mks:
	touch maketests.mk
	chmod +x generate.sh
	./generate.sh

-include maketests.mk

# Rules declared in add mk file
#ADDRULE="test_add"
#LISTRULE="test_list"

tests: tests_add #clean_slate tests_add

##
# Used to ensure fresh .mk and .csv files
clean_slate: mks 
tests_add: test_add

GSCRIPTS=lab2_add.gp lab2_list.gp

##
# TODO: possibly remove dependency on tests to allow user to use previously
# generated numbers
graphs: #tests
	./lab2_add.gp

dist:
	tar -cvzf lab2a-604853262.tar.gz ${DEFAULT} ${SOURCE} ${GSCRIPTS}

clean:
	rm -rf lab2_list lab2_add *tar.gz *.mk *.csv *.png


ctests: #build
	-./lab2_add --threads=2  --iterations=100    >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=1000   >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000  >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100    >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=1000   >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000  >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=100    >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=1000   >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000  >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=100000 >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100    >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000   >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000  >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100000  >> lab2_add.csv

	-./lab2_add --threads=2  --iterations=10     --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10     --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10     --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10     --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=20     --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=20     --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=20     --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=20     --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=40     --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=40     --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=40     --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=40     --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=80     --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=80     --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=80     --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=80     --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100    --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100    --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=100    --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100    --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=1000   --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=1000   --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=1000   --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000   --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000  --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000  --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000  --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000  --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=100000 --yield >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=100000 --yield >> lab2_add.csv

	-./lab2_add --threads=1  --iterations=100         >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=1000        >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=10000       >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=100000      >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=1000000     >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100         >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=1000        >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000       >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100000      >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=1000000     >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100         >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=1000        >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000       >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100000      >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=1000000     >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=100     --yield >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=1000    --yield >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=10000   --yield >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=100000  --yield >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=1000000 --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100     --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=1000    --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000   --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=100000  --yield >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=1000000 --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100     --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=1000    --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000   --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=100000  --yield >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=1000000 --yield >> lab2_add.csv

	-./lab2_add --threads=2  --iterations=10000 --yield --sync=m >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000 --yield --sync=m >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000 --yield --sync=m >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --yield --sync=m >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000 --yield --sync=c >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000 --yield --sync=c >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000 --yield --sync=c >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --yield --sync=c >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000 --yield --sync=s >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000 --yield --sync=s >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=1000  --yield --sync=s >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000  --yield --sync=s >> lab2_add.csv

	-./lab2_add --threads=1  --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=1  --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=2  --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=4  --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=8  --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv


	-./lab2_list --threads=1  --iterations=10	      >> lab2_list.csv
	-./lab2_list --threads=1  --iterations=100	      >> lab2_list.csv
	-./lab2_list --threads=1  --iterations=1000	      >> lab2_list.csv
	-./lab2_list --threads=1  --iterations=10000	      >> lab2_list.csv
	-./lab2_list --threads=1  --iterations=20000	      >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=1000           >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1000           >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1000           >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000           >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=i  >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=d  >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=il >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=dl >> lab2_list.csv

	-./lab2_list --threads=12 --iterations=32 --yield=i  --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d  --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=i  --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d  --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

	-./lab2_list --threads=1  --iterations=1000          >> lab2_list.csv
	-./lab2_list --threads=1  --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=1  --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

