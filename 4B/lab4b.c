// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

// NTS on 11-10-2018: pick up on line 88 - mraa_result_t mraa_gpio_isr

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "utils.h"

// include more headers here:
#include <time.h>
#include <fcntl.h>
#include <math.h>
//#include <ctype.h>


//============================================================
// 							DEFINE
//============================================================
#define PERIOD 'p'
#define LOG 'L'
#define SCALE 's'

#define FAHRENHEIT 'f'
#define CELSIUS 'c'

#define R_0 100000
#define B 4275
#define REF_TEMP 298.15 // in units Kelvin
#define KELVIN_OFFSET 273.15


// runtime commands
#define RT_SCALE_F "SCALE=F"
#define RT_SCALE_C "SCALE=C"
#define RT_PERIOD "PERIOD="
#define RT_STOP "STOP"
#define RT_START "START"
#define RT_LOG "LOG"
#define RT_OFF "OFF"


#define BUF_SIZE 1024

/* Use DUMMY to test lab4b on local machines versus testing on
 * beaglebone. Note that we can still enable error checking with
 * these dummies in a way that does not conflict with the live
 * implementation of each DUMMY
 */
#define DUMMY 1

#ifndef DUMMY
//#include <aio.h>
//#include <mraa/aio.h>
//#include <mraa/gpio.h>
#endif

#ifdef DUMMY
#define MRAA_GPIO_EDGE_RISING 0
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
// TODO: pickup here!!
/* gpio_isr is used to set interrupt pin. fptr is a pointer to the 
 * function to be used when a signal occurs, essentially a sig_handler.
 */
typedef int mraa_result_t;
typedef int mraa_gpio_edge_t;
mraa_result_t mraa_gpio_isr(mraa_gpio_context dev, mraa_gpio_edge_t edge,
							void(*fptr)(void *), void *args)
{
	mraa_result_t result = 1;
	return result;
}
#endif




//============================================================
//							MAIN
//============================================================
int running = 1;
int logging = 0;

int period = 1;
FILE* logstream = NULL;
char scale = FAHRENHEIT;

struct tm* gettime() {
	time_t raw_time;
	time(&raw_time);
	struct tm* time = localtime(&raw_time);
	return time;
}
void printtime(const struct tm* time) {
	printf("%02d:%02d:%02d ", time->tm_hour, time->tm_min, time->tm_sec);
}

float gettemp(mraa_aio_context* temp_sensor) {
	int raw_temp = mraa_gpio_read(*temp_sensor);
	float R = 1023.0/raw_temp - 1.0;
	R *= R_0;
	float temp = 1.0/(log(R/R_0)/B + 1/REF_TEMP) - KELVIN_OFFSET;

	if(scale == CELSIUS)
		return temp;
	else
		return 1.8*temp + 32.0;
}

void shutdown() {

}

/* In the event of a fatal error, or upon normal shutdown of main,
 * run cleanup to deallocate globally allocated memory.
 */
void cleanup() {
	if (logstream) {
		if(debug_flag) debug("closing logstream");
		if((fclose(logstream)) != 0) {
			fatal_error("error closeing logstream to logfile", NULL, 1);
		}
	}
}

void get_options(const int *argc, char* const* argv) {
	static struct option const long_opts[] = {
		{"period", optional_argument, NULL, PERIOD},
		{"log", optional_argument, NULL, LOG},
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
			case LOG: {
				if(optarg) {
					// USING A LOGSTREAM
					logstream = fopen((const char*)optarg, "w");
					if(logstream == NULL) {
						fatal_error("unable to create logfile", NULL, EXIT_ERROR1);
					}
					
					logging = 1;
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
	//if(logfile) fprintf(stderr, " logfile=%s", logfile);
	fprintf(stderr, "\n");
}

void usage(void) {
	fprintf(stderr, "Usage: ./%s --period=<seconds> --log=<filename> --scale=<[f, c]>\n", program_name);
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options((const int*)&argc, argv);

	mraa_aio_context temp_sensor;
	mraa_gpio_context button;
	temp_sensor = mraa_aio_init(1);
	button = mraa_gpio_init(62);

	/* In the event of an interrupt, gpio_isr will call either the
	 * shutdown routine or it will call a (later defined) sig_handler.
	 */
	mraa_gpio_dir(button, MRAA_GPIO_IN);
	mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &shutdown, NULL);


	if(fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1) {
		fatal_error("error setting stdin to be non-blocking", NULL, 1);
	}

	//char buf[BUF_SIZE];
	//char temp_buf[]

	

	if(debug_flag) print_options();
	cleanup();
	return 0;
}