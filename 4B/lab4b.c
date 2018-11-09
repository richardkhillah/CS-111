// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

#include "utils.h"

void usage(void) {
	fprintf(stderr, "Usage: ./%s \n", program_name);
}

int main(int argc, char* argv[]) {
	printf("Hello, world!\n");
	return 0;
}