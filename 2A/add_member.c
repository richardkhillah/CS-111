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

void usage() {
	fprintf(stderr, "Usage: ./%s [--threads=num] [--iterations=num] [--yield] [--sync=type]\n", program_name);
}


static struct option const long_opts[] = {
	{"threads", optional_argument, NULL, THREADS},
	{NULL, 0, NULL, 0}
};

void get_options(int argc, char* const* argv) {
	int opt;
	int optind;
	while( (opt = getopt_long(argc, argv, "t:", long_opts, &optind)) != -1 ) {
	switch(opt) {
		case THREADS:

			break;
		default:
			break;
	}
	} // end while
}