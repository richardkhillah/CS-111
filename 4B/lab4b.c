// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "utils.h"

// include more headers here:


// definitions
#define PERIOD 'p'
#define LOGFILE 'L'
#define SCALE 's'

#define FAHRENHEIT 'f'
#define CELSIUS 'c'

//#define DUMMY

#ifndef DUMMY
//#include <aio.h>
//#include <mraa/gpio.h>
//#include <mraa/aio.h>
#endif

#ifdef DUMMY
typedef int mraa_aio_context;
int mraa_aio_init(int input) {
	input++;
	return 100;
}
int mraa_aio_read(int temp) {
	temp++;
	return 500;
}

typedef int mraa_gpio_context;
int MRAA_GPIO_IN = 5;
int mraa_gpio_init(int val) {
	val++;
	return -5;
}
void mraa_gpio_dir(int val, int temp) {
	val++;
	temp++;
}
int mraa_gpio_read(int* val) {
	(*val)++;
	return *val;
}
#endif






int period = 1;
char* logfile = NULL;
char scale = FAHRENHEIT;


/* In the event of a fatal error, or upon normal shutdown of main,
 * run cleanup to deallocate globally allocated memory.
 */
void cleanup() {
	if(logfile) {
		if(debug_flag) debug("cleaning up logfile");
		free(logfile);
	}
}

void get_options(const int *argc, char* const* argv) {
	static struct option const long_opts[] = {
		{"period", optional_argument, NULL, PERIOD},
		{"logfile", optional_argument, NULL, LOGFILE},
		{"scale", optional_argument, NULL, SCALE},
		{"debug", no_argument, NULL, DEBUG},
		{NULL, 0, NULL, 0}
	};

	if(argc == NULL || argv == NULL) {
		cleanup();
		fatal_error("Error reading main arguments", NULL, EXIT_ERROR3);
	}

	int opt;
	int optind;
	while( (opt = getopt_long( *(int*)argc, argv, "", long_opts, &optind)) != -1 ) {
		switch (opt) {
			case PERIOD: {
				if(optarg) {
					int p = atoi(optarg);
					if(p < 1) {
						cleanup();
						fatal_error("period must be greater than 1", (void*)usage, EXIT_ERROR1);
					}
					period = p;
				} else {
					cleanup();
					fatal_error("internal error", NULL, EXIT_ERROR3);
				}
				break;
			}
			case LOGFILE: {
				if(optarg) {
					int buflen = strlen(optarg);
					if(buflen < 1) {
						cleanup();
						fatal_error("something's not right with the filename you passed in.", (void*)usage, EXIT_ERROR3);
					}

					logfile = (char*)malloc( (buflen+1)*sizeof(char) );
					if(logfile == NULL) {
						cleanup();
						fatal_error("internal error allocating memory for logfile", NULL, EXIT_ERROR1);
					}

					int i = 0;
					while(i < buflen) {
						logfile[i] = optarg[i];
						i++;
					}
					logfile[i] = '\0';
					//printf("logfile=%s\n", logfile);
				}
				break;
			}
			case SCALE: {
				if(optarg) {
					switch(*optarg) {
						case FAHRENHEIT: {
							scale = FAHRENHEIT;
							break;
						}
						case CELSIUS: {
							scale = CELSIUS;
							break;
						}
						default: {
							cleanup();
							fatal_error("invalid scale", (void*)usage, EXIT_ERROR1);
						}
					}
				} else {
					cleanup();
					fatal_error("internal error reading optarg", NULL, EXIT_ERROR3);
				}
				break;
			}
			case DEBUG: {
				debug_flag = 1;
				break;
			}
			case '?':
			default:
				cleanup();
				fatal_error("unrecognized argument", (void*)usage, 1);
		}
	}
}

void print_options() {
	fprintf(stderr, "option values for %s:", program_name);
	fprintf(stderr, " scale=%c", scale);
	fprintf(stderr, " period=%d", period);
	if(logfile) fprintf(stderr, " logfile=%s", logfile);
	fprintf(stderr, "\n");
}

void usage(void) {
	fprintf(stderr, "Usage: ./%s --period=numseconds --logfile=filename --scale=[f, c]\n", program_name);
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options((const int*)&argc, argv);
	printf("Hello, world!\n");





	if(debug_flag) print_options();
	cleanup();
	return 0;
}