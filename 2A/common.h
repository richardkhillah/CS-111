#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * set_program_name (and program_name) .. set program name using 
 * argv[0] of command line argument.
 * 
 * The program strips the './' from the command used to invoke the 
 * function and stores the result in program_name.
 *
 * @param const char* argv0 ... main routines argv[0]. 
 */
const char* program_name;

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


/**
 * 
 */