// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <getopt.h>

#include "common.h"

int main(int argc, char* argv[]) {
	set_program_name();
	printf("program_name: %s\n", program_name);
	return 0;
}