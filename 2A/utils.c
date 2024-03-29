// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include "utils.h"

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

//void fatal_error(char* msg, int usage, int errcode){
void fatal_error(char* msg, void (*usage)(void), int errcode){
    fprintf(stderr, "%s: %s\r\n", program_name, msg);
    

    if(usage != NULL) {
        (*usage)();
    }

    switch(errcode) {
        case EXIT_ERROR_1:
            exit(EXIT_ERROR_1);
        case EXIT_ERROR_2:
            exit(EXIT_ERROR_2);
        default:
            fprintf(stderr, "invalid error code... existing anyway\r\n");
            exit(EXIT_ERROR_ERROR);
    }
}

void handle_error(char* msg) {
    fprintf(stderr, "[%s]: %s (errno %d)\r\n", msg, strerror(errno), errno);
    exit(EXIT_ERROR_1);
}

void debug(char* msg) {
    fprintf(stderr, "[%s]: %s\r\n",program_name, msg);
}