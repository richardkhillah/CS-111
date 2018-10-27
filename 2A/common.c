// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include "common.h"
#include "add_member.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>
#include <string.h>

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

void fatal_error(char* msg){
    fprintf(stderr, "%s: %s\r\n", program_name, msg);
    usage();
    exit(EXIT_ERROR);
}

void fatal_error2(char* msg) {
    fprintf(stderr, "%s: %s\r\n", program_name, msg);
    usage();
    exit(EXIT_ERROR_OTHER);
}

void handle_error(char* msg) {
    fprintf(stderr, "[%s]: %s (errno %d)\r\n", msg, strerror(errno), errno);
    exit(EXIT_ERROR);
}

void debug(char* msg) {
    fprintf(stderr, "%s\r\n", msg);
}