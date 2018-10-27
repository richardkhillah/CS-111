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




void add(long long *pointer, long long value) {
	long long sum = *pointer + value;
	*pointer = sum;
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	printf("program_name: %s\n", program_name);
	usage();
	return 0;
}