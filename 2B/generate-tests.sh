#!/bin/bash

LIST=- ./lab2_list
OUT_FILE=lab2_list.csv

for threads in 1 2 4 8 12 16 24; do
	for sync_type in m s; do
		$LIST --threads=${THREADS} --iterations=1000 --sync=${sync_type} >> $OUT_FILE
	done
done
