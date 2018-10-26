// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>
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

void fatal_error(char* my_message){
    fprintf(stderr, "%s\r\n", my_message);
    exit(EXIT_ERROR);
}

void handle_error(char* msg, void* errcode) {
    fprintf(stderr, "[%s]: %s (errno %d)\r\n", msg, strerror(errno), errno);
    exit(EXIT_ERROR);
}

void debug(char* msg) {
    fprintf(stderr, "%s\r\n", msg);
}