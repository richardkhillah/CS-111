#!/bin/sh

ECHO=echo 
LIST=./lab2_list
OUT_FILE=lab2b_list.csv
for sync_type in m s; do
	for threads in 1 2 4 8 12 16 24; do
		$LIST --threads=${threads} --iterations=1000 --sync=${sync_type} >> $OUT_FILE
	done
done
