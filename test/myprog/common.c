#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

//#include <fcntl.h>
//#include <string.h>
//#include <errno.h>
//#include <getopt.h>

/* Common exit status' */
//#define EXIT_OK 0
//#define EXIT_ERROR 1
//#define EXIT_INT_ERROR 2



void print_error(char* msg) {
    if(msg) {
	fprintf(stderr, "print msg: %s\n", msg);
    } else {
	fprintf(stderr, "NULL passed in\n");
    }
    exit(1);
}
