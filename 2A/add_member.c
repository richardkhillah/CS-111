// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include "common.h"
#include "add_member.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

int numThreads = DEFAULT;
int numIterations = DEFAULT;
bool yield = false;
bool sync_flag = false;
char sync_type = NONE;

void handle_sig(int sig) {
	if(sig == SIGSEGV) {
		fatal_error("Segmentation fault", NULL, 1);
	}
}

void usage() {
	fprintf(stderr, "Usage: ./%s [--threads=num] [--iterations=num] [--yield] [--sync=type]\n", program_name);
}

void tag() {
	printf("add-\r\n");

	if(yield) printf("yield-\r\n");

	if(sync_type == NONE) printf("none,");
	else printf("%c,", sync_type);
}


static struct option const long_opts[] = {
	{"threads", required_argument, NULL, THREADS},
	{"iterations", required_argument, NULL, ITERATIONS},
	{"yield", no_argument, NULL, YIELD},
	{"sync", required_argument, NULL, SYNC},
	{NULL, 0, NULL, 0}
};

void get_options(int argc, char* const* argv) {
	int opt;
	int optind;
	while( (opt = getopt_long(argc, argv, "", long_opts, &optind)) != -1 ) {
	switch(opt) {
		case THREADS:
			numThreads = atoi(optarg);
			if (numThreads <= 0) {
				fatal_error("--threads: input a number greater than 0.", (void*)usage, 1);
			}
			break;
		case ITERATIONS:
			numIterations = atoi(optarg);
			if (numIterations <= 0) {
				fatal_error("--iterations: must be a number greater than 0.", (void*)usage, 1);
			}
			break;
		case YIELD:
			yield = true;
			break;
		case SYNC:
			if(optarg) {
				switch (*optarg) {
					case MUTEX:
						sync_type=MUTEX;
						break;
					case SPIN:
						sync_type=SPIN;
						break;
					case CAS:
						sync_type=CAS;
						break;
					default:
						fatal_error("--sync: Invalid option. Acceptable options are: m, s, c ", (void*)usage, 1);
				}
			}
			break;
		default:
			fatal_error("invalid option", (void*)usage, 1);
			break;
	}
	} // end while
}