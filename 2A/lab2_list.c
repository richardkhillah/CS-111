#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

#include <pthread.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <signal.h>

#include "SortedList.h"
#include "common.h"
#include "list_member.h"

#define SEC_TO_NS_CFACTOR 1000000000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int spinLock = 0;

SortedList_t* head;
SortedListElement_t* elements;

void * thread_routine(void* arg) {
	int id = *((int *)arg);

	int num_ops = numThreads * numIterations;

	// add items to list
	int i = id;
	for(; i < num_ops; i+=numThreads) {
		switch(sync_type) {
			case NONE:
				SortedList_insert(head, elements+i);
				break;
			case MUTEX: {
				if(pthread_mutex_lock(&lock) != 0) {
					fatal_error2("Error getting the lock");
				}
				SortedList_insert(head, elements+i);
				if(pthread_mutex_unlock(&lock) != 0) {
					fatal_error2("Error releasing the lock");
				}
			}
				break;
			case SPIN: {
				while(__sync_lock_test_and_set(&spinLock, 1));

				SortedList_insert(head, elements+i);

				__sync_lock_release(&spinLock);
			}
				break;
		}
	}

	// get the list length
	switch(sync_type) {
		case NONE:
			SortedList_length(head);
			break;
		case MUTEX: {
			if(pthread_mutex_lock(&lock) != 0) {
				fatal_error2("Error getting the lock");
			}
			SortedList_length(head);
			if(pthread_mutex_unlock(&lock) != 0) {
				fatal_error2("Error releasing the lock");
			}
		}
			break;
		case SPIN: {
			while(__sync_lock_test_and_set(&spinLock, 1));

			SortedList_length(head);

			__sync_lock_release(&spinLock);
		}
				break;
	}

	// delete items
	i = id;
	for(; i < num_ops; i+=numThreads) {
		switch(sync_type) {
			case NONE: {
				SortedListElement_t *el = SortedList_lookup(head, elements[i].key);
				if(el == NULL) {
					fatal_error2("Element not found. NULL element lookup.");
				}
				SortedList_delete(el);
			}
				break;
			case MUTEX: {
				if(pthread_mutex_lock(&lock) != 0) {
					fatal_error2("Error getting the lock");
				}

				SortedListElement_t *el = SortedList_lookup(head, elements[i].key);
				if(el == NULL) {
					fatal_error2("Element not found. NULL element lookup.");
				}
				SortedList_delete(el);
				
				if(pthread_mutex_unlock(&lock) != 0) {
					fatal_error2("Error releasing the lock");
				}
			}
				break;
			case SPIN: {
				while(__sync_lock_test_and_set(&spinLock, 1));

				SortedListElement_t *el = SortedList_lookup(head, elements[i].key);
				if(el == NULL) {
					fatal_error2("Element not found. NULL element lookup.");
				}
				SortedList_delete(el);

				__sync_lock_release(&spinLock);
			}
				break;
		}
	}

	return NULL;
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options(argc, argv);

	if(debug_flag) debug("before allocating head");
	head = (SortedList_t *)malloc(sizeof(SortedList_t));
	if( head == NULL ) {
		fatal_error("Error initializing list");
	}

	// should I do a circular list?
	head->next = head;
    head->prev = head;
    head->key = NULL;
	
	if(debug_flag) debug("about to make elements");
	int numElements = numThreads * numIterations;
	elements = (SortedListElement_t *)malloc(numElements * sizeof(SortedListElement_t));
	if( elements == NULL ) {
		fatal_error("Error initializing SortedList elements");
	}

	if(debug_flag) debug("about to run for loop");
	int i = 0;
	for (; i < numElements; i++) {
		if(debug_flag) debug("inside rand_ele_len for loop");
		//TODO: CHANGE THIS!!
		int random_ele_len = 3 + (rand() % 8);
		char* key = (char *)malloc(random_ele_len * sizeof(char));
		if(key == NULL) {
			fatal_error("Error creating random element");
		}

		int j = 0;
		for(; j < random_ele_len - 1; j++) {
			key[j] = (char)(rand() % 255 + 1); // TODO: THIS OK?
		}
		key[random_ele_len-1] = '\0'; // null terminate c-string

		elements[i].key = key;
		//free(key); // THIS OKAY?
	}

	if(debug_flag) debug("about to make threadPool");
	pthread_t* threadPool = (pthread_t *)malloc(numThreads * sizeof(pthread_t));

	if(threadPool == NULL) {
		fatal_error("Error creating thread pool");
	}

	if(debug_flag) debug("about to malloc tids");
	int* tids = (int *)malloc(numThreads * sizeof(int));
	if(tids == NULL) {
		fatal_error("Error creating thread id list");
	}

	// Get start time
	struct timespec time_start, time_end;
	clock_gettime(CLOCK_MONOTONIC, &time_start);

	if(debug_flag) debug("about to start thread routines");
	i = 0;
	for (; i < numThreads; i++) {
		if(pthread_create(threadPool + i, NULL, thread_routine, tids + i) != 0) {
			fatal_error2("Error creating threads");
		}
	}

	if(debug_flag) debug("about to join threads");
	i = 0;
	for(; i < numThreads; i++) {
		if (pthread_join(threadPool[i], NULL) != 0) {
			fatal_error2("Error joining threads");
		}
	}

	// Get end time
	clock_gettime(CLOCK_MONOTONIC, &time_end);

	if(debug_flag) debug("getting list length");
	int list_length = SortedList_length(head);
	if(list_length != 0) {
		fatal_error2("List is not list 0");
	}

	long long time_elapsed = (time_end.tv_sec - time_start.tv_sec) * SEC_TO_NS_CFACTOR;
	time_elapsed += time_end.tv_nsec;
	time_elapsed -= time_start.tv_nsec;

	long long numOperations = numThreads * numIterations * 2;
	long long time_average = time_elapsed / numOperations;

	tag();
	printf(",%d,%d,1,%lld,%lld,%lld\n", numThreads, numIterations, numOperations, time_elapsed, time_average);

	free(threadPool);
	free(tids);
	free(elements);
	//free(head);
	return 0;
}