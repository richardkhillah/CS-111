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
PROFILE=profile.out


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

# tests: build 
# 	-./generate-tests.sh

graphs: tests
	gnuplot lab2_list.gp

profile:
	CPUPROFILE=profile ./lab2_list --iterations=1000 --threads=12 --sync=s
	pprof ./lab2_list profile --list=thread_routine > profile.out

dist: graphs
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(LIST_SOURCE) $(GSCRIPTS) $(PNGS) $(CSVS) $(PROFILE)

clean:
	rm -rf lab2_list *tar.gz *.dSYM

check:
	./P2B_check.sh $(UID)


############################################################
## END DEVELOPER RULES
############################################################

tests: build
	-./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=10 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=20 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=40 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=80 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=20 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=40 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=80 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=20 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=40 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=80 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=20 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=40 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=80 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=10 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=20 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=40 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=80 --yield=id --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=10 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=20 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=40 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=80 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=20 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=40 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=80 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=20 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=40 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=80 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=20 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=40 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=80 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=10 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=20 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=40 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=80 --yield=id --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
