# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#!/bin/bash

# generate test input for makefile l2add_tests.mk

# if there are any previously generate makefiles or .csv files, 
# remove them before creating the .mk recipes.
ADDCSV="lab2_add.csv"
#ADDMK="lab2_add.mk"
ADDMK="l2add_tests.mk"

rm -f *.mk
rm -f lab2_add.csv
echo "l2add_tests:" >> ${ADDMK}
for threads in 1 2 4 8 16; do
	for iterations in 100 1000 10000 100000; do
		echo "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
		# the following declaration is for lnxsrv07
		#echo -e "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
	done
done

echo >> ${ADDMK}
#echo -e "" >> ${ADDFILE}

for threads in 1 2 4 8 16; do
	for iterations in 100 1000 10000 100000; do
		echo "\t-./lab2_add --threads=${threads} --iterations=${iterations} --yield >> lab2_add.csv" >> ${ADDMK}
		# the following declaration is for lnxsrv07
		#echo -e "\t-./lab2_add --threads=${threads} --iterations=${iterations} >> lab2_add.csv" >> ${ADDMK}
	done
done