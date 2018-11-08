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
#	lab2b_1.png ... operations per second vs number of threads 
#	lab2b_2.png ... 
#	lab2b_3.png ... 
#	lab2b_4.png ... 
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
set title "List-1: Operation per Second vs Number of Threads"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput"
set logscale y 10 
set output 'lab2b_1.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep 'list-none-m,[1-9]*,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'mutex' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,[1-9]*,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'spin-lock' with linespoints lc rgb 'green'



# how much time is spent waiting for locks
set title "List-2: Per Operation Times for Mutex Operations"
set xlabel "Threads"
set logscale x 2
set ylabel "Average Time per Operation"
set logscale y 10 
set output 'lab2b_2.png'

plot \
	 "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'Wait time for lock' with linespoints lc rgb 'red', \
	 "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'Operation time' with linespoints lc rgb 'green'



# Protected iterations that run without failure
set title "List-3: Successful iterations that run w/o Failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Successful iterations"
set logscale y 10
set output 'lab2b_3.png'

plot \
	 "<grep list-id-none lab2b_list.csv" using ($2):($3) \
	title 'No Synchronization' with points lc rgb 'red', \
	 "<grep list-id-m lab2b_list.csv" using ($2):($3) \
	title 'Mutex' with points lc rgb 'green', \
	 "<grep list-id-s lab2b_list.csv" using ($2):($3) \
	title 'Spin-lock' with points lc rgb 'blue'




# aggregated throughput (total operations per sec) vs num threads
set title "List-4: Mutex aggregate throughput vs number of threads with varying number of lists"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_4.png'

plot \
	 "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '1 List' with linespoints lc rgb 'red', \
     "< grep 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '4 Lists' with linespoints lc rgb 'green', \
     "< grep 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '8 Lists' with linespoints lc rgb 'blue' ,\
     "< grep 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '16 Lists' with linespoints lc rgb 'purple'


# aggregated throughput (total operations per sec) vs num threads
set title "List-5: Spin-lock aggregate throughput vs number of threads with varying number of lists"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_5.png'

plot \
	 "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '1 List' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '4 Lists' with linespoints lc rgb 'green', \
     "< grep 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '8 Lists' with linespoints lc rgb 'blue' ,\
     "< grep 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title '16 Lists' with linespoints lc rgb 'purple'