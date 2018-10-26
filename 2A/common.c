// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <string.h>

#include "common.h"

void set_program_name(const char* argv0) {
    if(argv0 == NULL) {
	fprintf(stderr, "%s: set_program_name: %s.\n",	\
		program_name, strerror(errno));
	abort();
    }

    const char* slash = strrchr(argv0, '/');
    const char* base = (slash != NULL ? slash + 1 : argv0);

    argv0 = base;

    program_name = argv0;
}