#! /usr/local/cs/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#
# output:
#	lab2b_list-1.png ... cost per operation vs threads and iterations
#	lab2b_list-2.png ... threads and iterations that run (un-protected) w/o failure
#	lab2b_list-3.png ... threads and iterations that run (protected) w/o failure
#	lab2b_list-4.png ... cost per operation vs number of threads
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

# how synchronization method affects the total number of operations per second
set title "List-1: Cost per Operation vs Number of Threads"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput"
set logscale y 10 
set output 'lab2b_list-1.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep 'list-none-m,[1-9]*,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'mutex' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,[1-9]*,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'spin-lock' with linespoints lc rgb 'green'

