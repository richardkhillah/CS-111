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


void* thread_routine() {
	/* add 1  */
	for(int i = 0; i < numIterations; i++) {
		add(&counter, 1);
	}

	/* add -1 */
	for(int i = 0; i < numIterations; i++) {
		add(&counter, -1);
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options(argc, argv);

	signal(SIGSEGV, handle_sig);
	tag();

	/* Initial long long counter to zero */
	counter = 0;

	pthread_t *threadPool;
	threadPool = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
	
	if(threadPool == NULL)
		fatal_error2("Error allocating thread pool");

	struct timespec t_start, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	/* set threads off running thread_routine */
	for(int i = 0; i < numThreads; i++) {
		if(pthread_create(threadPool + i, NULL, thread_routine, NULL) != 0)
			fatal_error2("Error creating a thread.");

	}

	/* rejoin threads to main*/
	for(int i = 0; i < numThreads; i++) {
		if(pthread_join(threadPool[i], NULL) != 0)
			fatal_error2("Error joining a thread.");

	}

	clock_gettime(CLOCK_MONOTONIC, &t_end);	

	return 0;
}