# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262
LAB_NUMBER=lab2
LAB_PART=b

CC=gcc
CFLAGS= -Wall -Wextra -pthread -std=gnu99 -g # -lprofiler

DEFAULT=Makefile README utils.c utils.h
UTILS=utils.c utils.h

LIST_SOURCE=lab2_list.c SortedList.h SortedList.c helper.h lab2_list-helper.c

GSCRIPTS=lab2_list.gp
PNGS= *.png
CSVS= *.csv

# default: build

# build: lab2_list

# lab2_list:
# 	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c lab2_list-helper.c utils.c

# tests: build
# 	-./generate-tests.sh	

# graphs: tests
# 	./lab2_list.gp

# profile:
# 	pprof ./lab2_list profile.out

# dist: graphs
# 	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(LIST_SOURCE) $(GSCRIPTS) $(PNGS) $(CSVS)

# clean:
# 	rm -rf lab2_list *tar.gz

# clean_all: clean
# 	rm -rf $(PNGS) $(CSVS)

# check:
# 	./P2B_check.sh $(UID)





############################################################
## DEVELOPER RULES
############################################################
default: build

build: lab2_list

lab2_list:
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c lab2_list-helper.c utils.c

tests: build 
	-./generate-tests.sh

graphs: 
	gnuplot lab2_list.gp

dist: graphs
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(LIST_SOURCE) $(GSCRIPTS) $(PNGS) $(CSVS)

profile:
	CPUPROFILE=profile ./lab2_list --iterations=1000 --threads=12 --sync=s
	pprof ./lab2_list profile --list=thread_routine > profile.out

clean:
	rm -rf lab2_list $(PNGS) $(CSVS) *tar.gz *.dSYM

check:
	./P2B_check.sh $(UID)


############################################################
## END DEVELOPER RULES
############################################################



# test_list: lab2_list
# 	-./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
# 	-./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
# 	-./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
# 	-./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
# 	-./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

# 	-./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

# 	-./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv

# 	-./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv

# 	-./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv

# 	-./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv


# 	-./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

# 	-./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
# 	-./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
# 	-./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
# 	-./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

