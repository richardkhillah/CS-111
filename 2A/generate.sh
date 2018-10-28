# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#!/bin/bash

# generate test input for makefile l2add_tests.mk

# if there are any previously generate makefiles or .csv files, 
# remove them before creating the .mk recipes.

ECHO="echo" # use on local machine
#ECHO="echo -e" # use on lnxsrv07

ADDCSV="lab2_add.csv"
ADDMK="maketests.mk"
#ADDMK="l2add_tests.mk"

ADDRULE="test_add"
LISTRULE="test_list"

rm -f *.mk
rm -f lab2_add.csv
$(ECHO) "${ADDRULE}:" >> ${ADDMK}

# NONE (--threads and --iterations only)
for threads in 1 2 4 8 16; do
	for iterations in 100 1000 10000 100000; do
		$(ECHO) "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
		# the following declaration is for lnxsrv07
		#$(ECHO) -e "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
	done
done

$(ECHO) >> ${ADDMK}
#$(ECHO) -e >> ${ADDFILE}

# YIELD (--threads, --iterations, --yield)
for threads in 1 2 4 8 12; do
	for iterations in 10 20 40 80 100 1000 10000 100000; do
		$(ECHO) "\t-./lab2_add --threads=${threads} --iterations=${iterations} --yield >> lab2_add.csv" >> ${ADDMK}
		# the following declaration is for lnxsrv07
		#$(ECHO) -e "\t-./lab2_add --threads=${threads} --iterations=${iterations} --yield >> lab2_add.csv" >> ${ADDMK}
	done
done
