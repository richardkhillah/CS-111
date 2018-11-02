# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#!/bin/bash

# generate test input for makefile l2add_tests.mk

# if there are any previously generate makefiles or .csv files, 
# remove them before creating the .mk recipes.

#ECHO="echo" # use on local machine
ECHO="echo -e" # use on lnxsrv07
ADDMK="maketests.mk"

ADDCSV="lab2_add.csv"
ADDRULE="test_add"


rm -f *.mk
rm -f lab2_add.csv
$ECHO "${ADDRULE}:" >> ${ADDMK}
# A
for threads in 1 2 4 8 12; do
	for iterations in 100 1000 10000 100000; do
		$ECHO "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
	done
done
$ECHO >> ${ADDMK}

# B
#no yield
for threads in 1 2 4 8 12; do
	for iterations in 10 20 40 80 100 1000 10000 100000; do
		$ECHO "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
	done
done
# yes yield
for threads in 2 4 8 12; do
	for iterations in 10 20 40 80 100 1000 10000 100000; do
		$ECHO "\t-./lab2_add --threads=${threads} --iterations=${iterations} --yield >> lab2_add.csv" >> ${ADDMK}
	done
done
$ECHO >> ${ADDMK}

# C
# no yield
for threads in 2 8; do
	for iterations in 100 1000 10000 100000; do
		$ECHO "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
	done
done
# yes yield
for threads in 2 8; do
	for iterations in 100 1000 10000 100000; do
		$ECHO "\t-./lab2_add --threads=${threads} --iterations=${iterations} --yield >> lab2_add.csv" >> ${ADDMK}
	done
done
$ECHO >> ${ADDMK}

# D
for iterations in 100 1000 10000 100000; do
	$ECHO "\t-./lab2_add --threads=1 --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
done
$ECHO >> ${ADDMK}

# E
# no yield
for threads in 1 2 4 8 12; do       
	for sync in m c s; do
		if [ ${sync} == s ]; then
			$ECHO "\t-./lab2_add --threads=${threads} --iterations=1000 --sync=${sync} >> lab2_add.csv" >> ${ADDMK}
		else
			$ECHO "\t-./lab2_add --threads=${threads} --iterations=10000 --sync=${sync} >> lab2_add.csv" >> ${ADDMK} 
		fi
	done
done
# yes yield
for threads in 2 4 8 12; do
	for sync in m c s; do
		if [ ${sync} == s ]; then
			$ECHO "\t-./lab2_add --threads=${threads} --iterations=1000 --sync=${sync} --yield >> lab2_add.csv" >> ${ADDMK}
		else
			$ECHO "\t-./lab2_add --threads=${threads} --iterations=10000 --sync=${sync} --yield >> lab2_add.csv" >> ${ADDMK} 
		fi
	done
done
$ECHO >> ${ADDMK}

# F
# no yield
for threads in 1 2 4 8 12; do
	$ECHO "\t-./lab2_add --threads=${threads} --iterations=10000 >> lab2_add.csv" >> ${ADDMK}
	for sync in m c s; do
		if [ ${sync} == s ]; then
			$ECHO "\t-./lab2_add --threads=${threads} --iterations=10000 --sync=${sync} >> lab2_add.csv" >> ${ADDMK}
		else
			$ECHO "\t-./lab2_add --threads=${threads} --iterations=10000 --sync=${sync} >> lab2_add.csv" >> ${ADDMK} 
		fi
	done
done


$ECHO >> ${ADDMK}
############################################################
$ECHO >> ${ADDMK}

# list tests

LISTCSV="lab2_list.csv"
LISTRULE="test_list"

rm -f lab2_list.csv
$ECHO "${LISTRULE}:" >> ${ADDMK}
for iterations in 10 100 1000 10000 20000; do
	$ECHO "\t-./lab2_list --threads=1 --iterations=${iterations} >> lab2_list.csv" >> ${ADDMK}
done
$ECHO >> ${ADDMK}

# B
for threads in 2 4 8 12; do
	for iterations in 1 10 100 1000; do
		$ECHO "\t-./lab2_list --threads=${threads} --iterations=${iterations} >> lab2_list.csv" >> ${ADDMK}
	done
done
$ECHO >> ${ADDMK}

#C
for yield in i d il dl; do
	for threads in 2 4 8 12; do
		for iterations in 1 2 4 8 16 32; do
			$ECHO "\t-./lab2_list --threads=${threads} --iterations=${iterations} --yield=${yield} >> lab2_list.csv" >> ${ADDMK}
		done
	done
	$ECHO >> ${ADDMK}
done
$ECHO >> ${ADDMK}

#D
for sync in m s; do
	for yield in i d il dl; do
		$ECHO "\t-./lab2_list --threads=12 --iterations=32 --yield=${yield} >> lab2_list.csv" >> ${ADDMK}
	done
done
$ECHO >> ${ADDMK}

#E
#$ECHO "\t-./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv" >> ${ADDMK}
for sync in m s; do
	for threads in 1 2 4 8 12 16 24; do
		$ECHO "\t-./lab2_list --threads=${threads} --iterations=1000 >> lab2_list.csv" >> ${ADDMK}
	done
done
$ECHO >> ${ADDMK}



