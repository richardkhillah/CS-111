// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

#include "common.h"
#include "list_member.h"
#include "SortedList.h"

bool debug_flag = false;

#define INSERT 'i'
#define DELETE 'd'
#define LOOKUP 'l'

int numThreads = DEFAULT;
int numIterations = DEFAULT;
int opt_yield = 0; 
bool sync_flag = false;
char sync_type = NONE;

void usage(void) {
	fprintf(stderr, "Usage: ./%s [--threads=num] [--iterations=num] [--yield=[idl]] [--sync=[s, m]]]\n", program_name);
}

static struct option const long_opts[] = {
	{"threads", required_argument, NULL, THREADS},
	{"iterations", required_argument, NULL, ITERATIONS},
	{"yield", required_argument, NULL, YIELD},
	{"sync", required_argument, NULL, SYNC},
	{"debug", no_argument, NULL, DEBUG},
	{NULL, 0, NULL, 0}
};

void get_options(int argc, char* const* argv) {
	int opt;
	int optind;
	while( (opt = getopt_long(argc, argv, "", long_opts, &optind)) != -1 ) {
		switch(opt) {
			case THREADS:
				numThreads = atoi(optarg);

				if(numThreads < 1)
					fatal_error("Number of threads must be greater than 0", (void*)usage, 1);
				break;
			case ITERATIONS:
				numIterations = atoi(optarg);

				if(numIterations < 1)
					fatal_error("Number of iterations must be greater than 0", (void*)usage, 1);
				break;
			case YIELD:
				if(optarg){	
					int i = 0;

					while(optarg[i] != '\0') {
						switch(optarg[i]) {
							case INSERT:
								opt_yield |= INSERT_YIELD;
								break;
							case DELETE:
								opt_yield |= DELETE_YIELD;
								break;
							case LOOKUP:
								opt_yield |= LOOKUP_YIELD;
								break;
							case '?':
							default:
								fatal_error("Invalid `--yield` argument", (void*)usage, 1);
						}
						i++;
					}
				} else {
					fatal_error("Must declare a valid `--yield` option", (void*)usage, 1);
				}
				break;

			case SYNC:
				if(optarg) {
					switch(*optarg) {
						case MUTEX:
							sync_type = MUTEX;
							break;
						case SPIN:
							sync_type = SPIN;
							break;
						default:
							fatal_error("Invalid --sync option", (void*)usage, 1);
					}
				}
				break;
			case DEBUG:
				debug_flag = true;
				break;
			case '?':
			default:
				fatal_error("Invalid argument", (void*)usage, 1);
		}
	}
}

void tag() {
	printf("list-");
	if(opt_yield) {
		if(opt_yield & INSERT_YIELD) {
			printf("i");
		}

		if(opt_yield & DELETE_YIELD) {
			printf("d");
		}

		if(opt_yield & LOOKUP_YIELD) {
			printf("l");
		}
	} else {
		printf("none");
	}

	printf("-");
	
	if(sync_type != NONE) {
		printf("%c", sync_type);
	} else {
		printf("none");
	}
}

void handle_sig(int sig) {
	if(sig == SIGSEGV) {
		fatal_error("Segmentation fault", 0, 1);
	}
}


