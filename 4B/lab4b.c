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
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>

#include <aio.h>
#include <poll.h>

//============================================================
// 							DEFINE
//============================================================

#define ferr1(m) fatal_error(m, NULL, 1);
#define ferr1u(m) fatal_error(m, (void*)usage, 1);

#define PERIOD 'p'
#define LOG 'l'
#define SCALE 's'

#define FAHRENHEIT_C 'F'
#define CELSIUS_C 'C'
#define FAHRENHEIT "F"
#define CELSIUS "C"

// Global Constants
const int BUF_SIZE = 2048;
const long SUCCESS_CODE = 0;
const long ERR_CODE = 1;
const long FAIL_CODE = 2;

#ifdef DUMMY
// Mock implementations for local dev/test
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
	return -60;
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


//======================================================================
// REMOVE THIS
//======================================================================
// INPUT: Name of sys call that threw error
// Prints reason for error and terminates program
void process_failed_sys_call(const char syscall[]) {
	int err = errno;
	fprintf(stderr, "%s", "An error has occurred.\n");
	fprintf(stderr, "The system call '%s' failed with error code %d\n", syscall, err);
	fprintf(stderr, "This error code means: %s\n", strerror(err));
	exit(ERR_CODE);
}
//======================================================================
// END REMOVE
//======================================================================



// INPUT: File stream to write to
// Print out hour:min:sec of current local time
void print_curr_time(FILE* fd) {
	time_t rawtime;
	time(&rawtime);
	if (rawtime == (time_t)-1) {
		ferr1("Error getting rawtime");
	}

	struct tm* local_time = localtime(&rawtime);
	if (local_time == NULL) {
		ferr1("Error getting rawtime");
	}

	fprintf(fd, "%02d:%02d:%02d ", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

	// if (local_time->tm_hour < 10)
	// 	fprintf(fd, "0");
	// fprintf(fd, "%d:", local_time->tm_hour);
	// if (local_time->tm_min < 10)
	// 	fprintf(fd, "0");
	// fprintf(fd, "%d:", local_time->tm_min);
	// if (local_time->tm_sec < 10)
	// 	fprintf(fd, "0");
	// fprintf(fd, "%d ", local_time->tm_sec);
}

// INPUT: Analog reading from temperature sensor
// Uses algorithm to convert analog reading to temperature
float convert_analog_to_temp(int analog, char* temp_unit) {
	float raw_value = (1023.0 / analog - 1.0) * 100000;
	float temperature = 1.0 / (log(raw_value / 100000) / 4275 + 1 / 298.15) - 273.15;

	if (*temp_unit == 'F') {
		return ((temperature * 9) / 5) + 32;
	}
	return temperature;
}

// INPUT: Temperature pin, temperature unit, logfile
// Create temperature file according to parameters and report it
void generate_temp_report(int temp_pin, char* scale, char* log) {
	// Sample temperature sensor and convert reading to indicated temperature
	int temp_analog_value = mraa_aio_read(temp_pin);
	if (temp_analog_value == -1) {	
		ferr1("Error reading temperature from temp_sensor");
	}
	float temperature = convert_analog_to_temp(temp_analog_value, scale);

	// Print report to stdout
	print_curr_time(stdout);
	fprintf(stdout, "%0.1f\n", temperature);

	// Append report to logfile
	if (log) {
		FILE* fd = fopen(log, "a+");
		if (fd == NULL) {
			ferr1("Failed to open logfile");
		}

		print_curr_time(fd);
		fprintf(fd, "%0.1f\n", temperature);

		fclose(fd);
	}
}

// INPUT: Logfile to write to
// Report shutdown message and exit
void shutdown(char* log) {
	print_curr_time(stdout);
	fprintf(stdout, "%s\n", "SHUTDOWN");

	if (log) {
		FILE* fd = fopen(log, "a+");
		if (fd == NULL) {
			ferr1("Failed to open logfile");
		}

		print_curr_time(fd);
		fprintf(fd, "%s\n", "SHUTDOWN");

		fclose(fd);
	}
	// mraa_gpio_close(button_pin);
	// mraa_aio_close(temp_pin);
	exit(SUCCESS_CODE);
}

void process_command(char* command, char** scale, int* delay, int* report, char* log) {
	if (strstr(command, "SCALE") != NULL)
	{
		if (strstr(command, FAHRENHEIT) != NULL) {
			//char* temp_scale = (char *) malloc(sizeof(char) * 2);
			//temp_scale[0] = FAHRENHEIT_C;
			//*scale = temp_scale;
			*scale = FAHRENHEIT;
		} else {
			//char* temp_scale = (char *) malloc(sizeof(char) * 2);
			//temp_scale[0] = CELSIUS_C;
			//*scale = temp_scale;
			*scale = CELSIUS;
		}
	}
	else if (strstr(command, "PERIOD") != NULL) {
		int i = 0;
		while (command[i++] != '=') ;//{ i++; }
		*delay = atoi(command + i + 1);
	}
	else if (strstr(command, "STOP") != NULL) { *report = 0; }
	else if (strstr(command, "START") != NULL) { *report = 1; }
	else if (strstr(command, "OFF") != NULL) { shutdown(log); }
}

//======================================================================
//
//							HELPER FUNCTIONS
//
//======================================================================

// INPUT: Info about CL arguments, strings for argument parameters
// Process CL arguments while checking for invalid options
void get_options(int argc, char** argv, char** period, char** temp, char** logfile) {
	static struct option const long_options[] = {
		{"period", required_argument, NULL, 'p'},
		{"scale", required_argument, NULL, 's'},
		{"log", required_argument, NULL, 'l'},
		#ifdef DEV
		{"debug", no_argument, NULL, DEBUG},
		#endif
		{0, 0, 0, 0}
	};
	int optind = 0;

	*period = "1";
	*temp = FAHRENHEIT;

	while (1) {
	  	int arg = getopt_long(argc, argv, "", long_options, &optind);

		if (arg == -1)
			return;

		switch (arg) {
			case PERIOD:
				*period = optarg;
				break;
			case SCALE:
				*temp = optarg;
				break;
			case LOG:
				*logfile = optarg;
				break;
			#ifdef DEV
			case DEBUG:
				fprintf(stderr, "setting debug_flag\n");
				debug_flag = 1;
				break;
			#endif
			case '?':
				ferr1u("Invalid argument");
		}
	}
}

void usage(void) {
	fprintf(stderr, "Usage: ./%s --period=<seconds> --log=<filename> --scale=<[f, c]>\n", program_name);
}

int main(int argc, char** argv) {
	set_program_name(argv[0]);


	char* period = NULL;
	char* scale = NULL;
	char* log = NULL;

	get_options(argc, argv, &period, &scale, &log);
	if(debug_flag) ferr1u("testing debug_flag!!");


	// Connect to temperature sensor
	mraa_aio_context temp_pin = mraa_aio_init(1);
	if (temp_pin == 0) { // Change to NULL
		ferr1("Error initializing the temperature sensor");
	}

	// Connect to button
	mraa_gpio_context button_pin = mraa_gpio_init(3);
	if (button_pin == 0) { //  Change to NULL
		ferr1("Error initializing the button");
	}
	mraa_gpio_dir(button_pin, MRAA_GPIO_IN);

	// Set up poll
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	int delay = atoi(period);
	int report = 1;

	//char buf[2014];
	char buf[BUF_SIZE];
	memset((char *) &buf, 0, sizeof(buf));
	while (1)
	{
		if (poll(fds, 1, 0) < 0) {
			ferr1("Failed to poll");
		}

		if (report) 
			generate_temp_report(temp_pin, scale, log);

		if (fds[0].revents & POLLIN)
		{
			int bytes_read = read(0, buf, sizeof(buf));
			if (bytes_read < 0) {
				ferr1("Failed to read stdin");
			}

			char* command = strtok(buf, "\n");
			while (command != NULL && bytes_read > 0) {
				if (log) {
					FILE* fd = fopen(log, "a+");
					if (fd == NULL) {
						ferr1("Failed to open logfile");
					}

					fprintf(fd, "%s\n", command);
				}

				process_command(command, &scale, &delay, &report, log);
				
				command = strtok(NULL, "\n");
			}
		}

		// Sample button state and check whether to exit
		if (mraa_gpio_read(&button_pin) == 1) {
			shutdown(log);
		}

		// Sampling interval
		sleep(delay);
	}
}


