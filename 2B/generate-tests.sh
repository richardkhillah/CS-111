#!/bin/sh

ECHO="echo"
LIST="./lab2_list"
OUT_FILE="lab2b_list.csv"
TEST_FILE="tests.mk"
for sync_type in m s; do
	for threads in 1 2 4 8 12 16 24; do
		$ECHO "-./lab2_list --threads=${threads} --iterations=1000 --sync=${sync_type} >> lab2b_list.csv" >> ${TEST_FILE}
	done
done

for threads in 1 4 8 12 16; do
	for iterations in 1 2 4 8 16; do
		$ECHO "-./lab2_list --threads=${threads} --iterations=${iterations} --yield=id --lists=4 >> lab2b_list.csv" >> ${TEST_FILE}
	done
done

for sync in s m; do
	for threads in 1 4 8 12 16; do
		for iterations in 10 20 40 80; do
			$ECHO "-./lab2_list --threads=${threads} --iterations=${iterations} --yield=id --sync=${sync} --lists=4 >> lab2b_list.csv" >> ${TEST_FILE}
		done
	done
done


# synchonized without yields with set iterations, varying threads and list numbers
for sync in s m; do
	for threads in 1 2 4 8 12; do
		for lists in 1 4 8 16; do
			$ECHO "-./lab2_list --threads=${threads} --iterations=1000 --sync=${sync} --lists=${lists} >> lab2b_list.csv" >> ${TEST_FILE}
		done
	done
done

