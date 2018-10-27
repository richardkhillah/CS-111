// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include "common.h"
#include "add_member.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <time.h>


long long counter;

void add(long long *pointer, long long value) {
	long long sum = *pointer + value;
	*pointer = sum;
}



int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options(argc, argv);

	tag();

	/* Initial long long counter to zero */
	counter = 0;

	pthread_t *threadPool;
	threadPool = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
	
	if(threadPool == NULL)
		fatal_error2("Error allocating thread pool");

	struct timespec t_start, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	/**/


	clock_gettime(CLOCK_MONOTONIC, &t_end);	

	return 0;
}