// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#ifndef AAA
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

#ifndef DUMMY
#include <mraa/aio.h>
#include <mraa/gpio.h>
#endif

#define ferr1(m) fatal_error(m, NULL, 1);
#define ferr1u(m) fatal_error(m, (void*)usage, 1);

#define PERIOD 'p'
#define LOG 'l'
#define SCALE 's'

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
int mraa_gpio_read(int val) {
	val++;
	return val;
}
#endif

char FAHRENHEIT_C = 'F';
char CELSIUS_C = 'C';
char* FAHRENHEIT_S = "F";
char* CELSIUS_S = "C";

// Global Constants
const int BUF_SIZE = 2048;
const int FILENAME_SIZE = 50;
const long SUCCESS_CODE = 0;
const long ERR_CODE = 1;
const long FAIL_CODE = 2;

#ifdef DEV

struct l4b_context {
	int state;			/* s = 1: recording data
							 	 * 2 = 0: not recording data   */
	struct tm* localtime;		/* raw time converted to local time */
	float temp;			/* raw temperature value converted to appropiate scale */
	char* temp_scale;/* in either degrees Fahrenheit or degrees Celsius */
	int sample_period;	/* interval between temperature sensor readings */
	char* logfile_name;			/* name of file to open/create in which to write
								 * temperature report to. */
	FILE* logfile_stream;		/* */
	int rt_cmd_len;				/* length of rt_command_buf */
	char* rt_cmd;				/* stoarage to hold commands passed in during runtime
								 * that should be printed to log_file.
								 * if a standard command like START, SCALE=, etc, buf
								 */
};
typedef struct l4b_context l4b_context_t;

// FUNCTION PROTOTYPES
void l4b_init(l4b_context_t** c);
void l4b_conext_update(l4b_context_t* c);
void l4b_get_rtcmd(char* cmd, l4b_context_t* c); 
// int l4b_get_rtcmd(l4b_context_t* c); 
void l4b_report(l4b_context_t* c);
void l4b_shutdown(l4b_context_t* c);
float raw_to_temp(int rv, char* tscale);
struct tm* get_time(void);
float get_temp(int temp_pin, char* tscale);

void print_context(l4b_context_t* c);


//================================================================================
//
//								API
//
//================================================================================
/* 
 *
 */
void l4b_init(l4b_context_t** c) {
	l4b_context_t* context = (l4b_context_t*)malloc(sizeof(l4b_context_t));
	context->state = 1;
	context->localtime = get_time();
	context->temp = 0.0;
	context->temp_scale = FAHRENHEIT_S;
	context->sample_period = 1;
	context->logfile_name = NULL;
	context->logfile_stream = NULL;
	context->rt_cmd_len = 0;
	context->rt_cmd = (char*)malloc(sizeof(char)*BUF_SIZE);
	if(context->rt_cmd == NULL) {
		ferr1("Error initializing rt_cmd buffer");
	}
	*c = context;
}

/* Takes a pre-linefeed-deliminated command and checks against "legal" run time
 * commands. If the command is valid, l4b_context c is updated. If command is 
 * not valid, nothing is done and program continues, silently "ignoring" 
 * unrecognized commands.
 * 
 * @param cmd 		pre-deliminated command
 */
void l4b_get_rtcmd(char* cmd, l4b_context_t* c) {
//int l4b_get_rtcmd(l4b_context_t* c) {
	//char* cmd = (char*)malloc(sizeof(char)*(c->rt_cmd_len));
	// if(cmd==NULL) ferr1("Error mallocing cmd in l4b_get_rtcmd");
	// strncpy(cmd, c->rt_cmd, c->rt_cmd_len);
	int match = 1;
	if (strstr(cmd, "SCALE") != NULL) {
		if (strstr(cmd, FAHRENHEIT_S) != NULL) {
			c->temp_scale = FAHRENHEIT_S;
			//fprintf(c->logfile_stream, "%s\n", "SCALE=F");
		} else {
			c->temp_scale = CELSIUS_S;
			//fprintf(c->logfile_stream, "%s\n", "SCALE=C");
		}
	}
	else if (strstr(cmd, "PERIOD") != NULL) {
		int i = 0;
		while (cmd[i] != '=') { i++; }
		c->sample_period = atoi(cmd + i + 1);
		//fprintf(c->logfile_stream, "PERIOD=%d\n", c->sample_period);
	}
	else if (strstr(cmd, "LOG") != NULL) {
		// int len = strlen(cmd);
		// int adjusted_len = len - 3; // strlen("LOG")

		// strncpy(c->rt_cmd, cmd+3, adjusted_len);
		//fprintf(c->logfile_stream, "%s\n", cmd);
	} 
	else if (strstr(cmd, "STOP") != NULL) { 
		c->state = 0; 
		// PICK UP HERE
	}
	else if (strstr(cmd, "START") != NULL) { c->state = 1; }
	else if (strstr(cmd, "OFF") != NULL) { l4b_shutdown(c); }
	else { match = 0; }

	if(match == 1) {
		c->rt_cmd = cmd;
		//fprintf(c->logfile_stream, "%s\n", c->rt_cmd);
	}
	//return match;

}

/* Reports to stdout temperature readings. If lab4b is started with the 
 * --log=<filename> option, temperature readings and run-time 
 * commands are written to a the user-specified file.
 *
 * @param c 		Determines what and where to write information.
 * @param rt_cmd 	'\n' deliminated command terminated with '\0'.
 */
void l4b_report(l4b_context_t* c) {
	// print to stdoud
	printf("%02d:%02d:%02d %0.1f\n", c->localtime->tm_hour, c->localtime->tm_min, c->localtime->tm_sec, c->temp);

	// print to logfile
	if(c->logfile_stream != NULL){
		fprintf(c->logfile_stream, "%02d:%02d:%02d %0.1f\n", c->localtime->tm_hour, c->localtime->tm_min, c->localtime->tm_sec, c->temp);
	}
}

void l4b_shutdown(l4b_context_t* c) {
	if(c->logfile_stream != NULL) {
		//l4b_report(c);
		struct tm* t = get_time();
		fprintf(c->logfile_stream, "%02d:%02d:%02d SHUTDOWN\n", t->tm_hour, t->tm_min, t->tm_sec);
	}
	exit(0);
}

//================================================================================
//
//								HELPER FUNCTIONS
//
//================================================================================
void usage(void) {
	fprintf(stderr, "Usage: ./%s --period=<seconds> --log=<filename> --scale=<[f, c]>\n", program_name);
}

void get_options(int argc, char* const* argv, l4b_context_t* context){
	static struct option const long_options[] = {
		{"period", required_argument, NULL, 'p'},
		{"scale", required_argument, NULL, 's'},
		{"log", required_argument, NULL, 'l'},
		#ifdef DEV
		{"debug", no_argument, NULL, DEBUG},
		#endif
		{NULL, 0, NULL, 0}
	};

	int optind = 0;
	int opt;
	while ((opt = getopt_long(argc, argv, "", long_options, &optind)) != -1) {
		switch (opt) {
			case PERIOD:
				context->sample_period = atoi(optarg);
				break;
			case SCALE:
				context->temp_scale = optarg;
				break;
			case LOG:
				context->logfile_name = optarg;
				break;
			#ifdef DEV
			case DEBUG:
				debug_flag = 1;
				break;
			#endif
			case '?':
				ferr1u("Invalid argument");
		}
	}
}

/* 
 * @return current localtime
 */
struct tm* get_time(void) {
	time_t rawtime;
	time(&rawtime);
	if (rawtime == (time_t)-1) {
		ferr1("Error getting rawtime");
	}

	struct tm* local_time = localtime(&rawtime);
	if (local_time == NULL) {
		ferr1("Error getting rawtime");
	}
	return (struct tm*)local_time;
}

/* @param temp_pin	beaglebone pin the temperature sensor is connected to
 * @param tscale 	The scale, in F(ahrenheit) or C(elsius) to report
 * @return			The converted rawvalue temperature read from temperature sensor
 */
float get_temp(int temp_pin, char* tscale){

	int pin_reading = mraa_aio_read(temp_pin);
	if (pin_reading == -1) {	
		ferr1("Error reading temperature from temp_sensor");
	}

	float raw_value = (1023.0 / pin_reading - 1.0) * 100000;
	float temperature = 1.0 / (log(raw_value / 100000) / 4275 + 1 / 298.15) - 273.15;

	if (strstr("F", tscale) != NULL) {
		return ((temperature * 9) / 5) + 32;
	}

	return temperature;
}



//================================================================================
//
//									MAIN
//
//================================================================================
int main(int argc, char* argv[]) {
	set_program_name(argv[0]);

	l4b_context_t* context;
	l4b_init(&context);
	get_options(argc, argv, context);

	if(context->logfile_name != NULL) {
		context->logfile_stream = fopen(context->logfile_name, "a+");
		if(debug_flag == 1){
			fprintf(context->logfile_stream, "printing a message to logfile\n");
		}
	}

	// Connect to temperature sensor
	mraa_aio_context temp_pin = mraa_aio_init(1);
	if (temp_pin == 0)
		ferr1("Error initializing the temperature sensor");

	// Connect to button
	mraa_gpio_context button_pin = mraa_gpio_init(60);
	if (button_pin == 0)
		ferr1("Error initializing the button");

	mraa_gpio_dir(button_pin, MRAA_GPIO_IN);


	#ifdef TEST
	test_rtcmd(context);
	print_context(context);

	test_context(context);
	l4b_report(context);
	#endif


	// polling apparatus
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	//char* buf = (char*)calloc(BUF_SIZE, sizeof(char));
	char* buf = (char*)malloc(BUF_SIZE*sizeof(char));
	// char* tmp = (char*)calloc(2*BUF_SIZE, sizeof(char));
	char* end;

	// int bufindex = 0;
	// int tmpindex = 0;

	while (1) {
		fprintf(stderr, "starting loop\n");
		fprintf(stderr, "==================================================\n");
		if (poll(fds, 1, 0) < 0) {
			ferr1("Error polling");
		}

		if (context->state == 1) {
			context->localtime = get_time();
			context->temp = get_temp(temp_pin, context->temp_scale);
			l4b_report(context);
		}

		if (fds[0].revents & POLLIN)
		{
			int bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
			if (bytes_read < 0) {
				ferr1("Failed to read stdin");
			}
			/*
				// while(bufindex < BUF_SIZE) {
				// 	//tmp[tmpindex] = buf[bufindex];
				// 	if(buf[bufindex] == '\n') {
				// 		// process command 
				// 		int diff = bufindex-tmpindex;
				// 		//strncpy(tmp, buf+bufindex, diff);
				// 		strncpy(context->rt_cmd, buf+diff, diff);
				// 		context->rt_cmd_len = diff;
				// 		//l4b_get_rtcmd(tmp, context);

				// 		fprintf(stderr, "%s\n", buf+bufindex);
				// 		fprintf(stderr, "%s\n", context->rt_cmd);
				// 		if(l4b_get_rtcmd(context) == 1) {
				// 			fprintf(context->logfile_stream, "%s\n", context->rt_cmd);
				// 		}

				// 		// reset tmp index
				// 		tmpindex = -1;
				// 	}
				// 	bufindex++;
				// 	tmpindex++;
				// }
				// bufindex = 0;
			*/

			char* command = strtok_r(buf, "\n", &end);
			int command_num = 1;
			fprintf(stderr, "command # %d: %s\n", command_num,command);
			while (command != NULL) {// && bytes_read > 0) {
				command_num++;
				fprintf(stderr, "command # %d: %s\n", command_num, command);
				// if (context->logfile_stream) {
				// 	fprintf(context->logfile_stream, "%s\n", command);
				// }

				l4b_get_rtcmd(command, context);
				fprintf(context->logfile_stream, "%s\n", context->rt_cmd);
				
				command = strtok_r(NULL, "\n", &end);
			}
		}

		// Sample button state and check whether to exit
		if (mraa_gpio_read(button_pin) == 1) {
			l4b_shutdown(context);
		}
		fprintf(stderr, "about to go to sleep\n");
		sleep(context->sample_period);
		fprintf(stderr, "just wokeup\n");
	}

	//free(tmp);
	free(buf);
	if(context->logfile_stream != NULL) {
		fprintf(stderr, "closing stream\n");
		fclose(context->logfile_stream);
	}
	free(context->rt_cmd);
	free(context);
	exit(0);
}

void test_rtcmd(l4b_context_t* context) {
	char* tmp = "Hello, world";
	int i = 0;
	while(tmp[i] != '\0') {
		context->rt_cmd[i] = tmp[i];
		i++;
	}
	i++;
	context->rt_cmd[i] = '\0';
	context->rt_cmd_len = i;

	int l = strlen(tmp);
	fprintf(stderr, "strlen(tmp) = %d\n", l);

	// int len = context->rt_cmd_len;
	// printf("iteratively printing characters in rt_cmd using percent c:\n");
	// for(int j = 0; j < len; j++) {
	// 	printf("%c", context->rt_cmd[j]);
	// }
	// printf("\n");

	// printf("printing rt_cmd directly using percent s:\n");
	// printf("%s\n", context->rt_cmd);
}

void print_context(l4b_context_t* context) {
	fprintf(stderr, "CONTEXT PRINTOUT\n");
	fprintf(stderr, "========================================\n");
	fprintf(stderr, "state: %d\n", context->state);
	fprintf(stderr, "localtime: %02d:%02d:%02d\n", context->localtime->tm_hour, context->localtime->tm_min, context->localtime->tm_sec);
	fprintf(stderr, "temp: %0.1f\n", context->temp);
	fprintf(stderr, "temp_scale: %s\n", context->temp_scale);
	fprintf(stderr, "sample_period: %d\n", context->sample_period);
	fprintf(stderr, "logfile_name: %s\n", context->logfile_name);
	fprintf(stderr, "rt_cmd_len: %d\n", context->rt_cmd_len);
	if(context->rt_cmd != NULL) {
		fprintf(stderr, "rt_cmd: %s\n", context->rt_cmd);
	}
	fprintf(stderr, "========================================\n");
}

void test_context(l4b_context_t* context) {
	mraa_aio_context temp_pin = mraa_aio_init(1);
	if (temp_pin == 0) { // Change to NULL
		ferr1("Error initializing the temperature sensor");
	}
	context->localtime = get_time();
	context->state = 1;
	context->temp_scale = CELSIUS_S;
	context->temp = get_temp(temp_pin, context->temp_scale);
	context->sample_period = 2;
	if(context->logfile_name == NULL) context->logfile_name = "logname";

	if(debug_flag == 1) print_context(context);
}


#endif // DEV

#endif// AAA


#ifdef AAA

#ifndef DUMMY
#include <mraa/aio.h>
#include <mraa/gpio.h>
#endif

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
int mraa_gpio_read(int val) {
	val++;
	return val;
}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "utils.h"

// include more headers here:
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <aio.h>


#define B 4275
#define R0 100000
#define F 'f'
#define C 'c'
#define PERIOD_FLAG 'p'
#define LOG_FLAG 'l'
#define SCALE_FLAG 's'
#define BUFFER_SIZE 1024

int period = 1;
char scale = F;
bool report = true;
FILE* logFile = NULL;
bool logging = false;


float getTemperature(int reading) {
    float r = 1023.0/reading-1.0;
    r = R0*r;

    float t =  1.0/(log(r/R0)/B+1/298.15)-273.15;
    if (scale == C) {
        return t;
    } else {
        return 1.8*t + 32;
    }
}

struct tm *printTime() {
    time_t rawTime;
    time(&rawTime);
    struct tm *t = localtime(&rawTime);
    printf("%02d:%02d:%02d ", t->tm_hour, t->tm_min, t->tm_sec);
    return t;
}

void shutDown() {
    struct tm* t = printTime();
    printf("SHUTDOWN\n");

    if (logging) {
        if (fprintf(logFile, "%02d:%02d:%02d SHUTDOWN\n", t->tm_hour, t->tm_min, t->tm_sec) < 0) {
            fprintf(stderr, "There was a problem writing to output\n");
            exit(1);
        } 
    }
    exit(0);
}

void usage(void) {
	fprintf(stderr, "Usage: ./%s --period=<seconds> --log=<filename> --scale=<[f, c]>\n", program_name);
}

void getoptions(int argc, char* const* argv){
	int optind = 1;
    int opt;
    struct option long_options[] = {
        {"period", required_argument, NULL, PERIOD_FLAG},
        {"log", required_argument, NULL, LOG_FLAG},
        {"scale", required_argument, NULL, SCALE_FLAG},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "p:l:s:", long_options, &optind)) != -1) {
        switch (opt) {
            case PERIOD_FLAG:
                if (optarg) {
                    period = atoi(optarg);
                    if (period < 1) {
                    	fatal_error("Period must be greater-than or equal to 0", (void*)usage, 1);
                    }
                }
                break;
            case LOG_FLAG:
                if (optarg) {
                    logFile = fopen(optarg, "w");
                    if (logFile == NULL) {
                    	fatal_error("Could not open file", NULL, 1);
                    }
                    logging = true;
                }
                break;
            case SCALE_FLAG:
                if (optarg) {
                    optarg[0] = tolower(optarg[0]);
                    if (strcmp(optarg, "c") != 0 && strcmp(optarg, "f")) {
                    	fatal_error("unknown scale... scale must be \"F\" or \"C\"", (void*)usage, 1);
                    }

                    scale = tolower(optarg[0]);
                }
                break;
            case '?':
            default:
            	fatal_error("unrecognized argument", (void*)usage, 1);
        }
    }

}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	getoptions(argc, (char* const*)argv);

    mraa_gpio_context button;
    mraa_aio_context tempSensor;
    button = mraa_gpio_init(60);
    tempSensor = mraa_aio_init(1);

    mraa_gpio_dir(button, MRAA_GPIO_IN);
    #ifndef DUMMY
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &shutDown, NULL);
    #endif

    char buffer[BUFFER_SIZE];
    char temp[BUFFER_SIZE*2];
    int tempindex = 0;
    int bufferindex = 0;

    // set keyboard to non-block
    if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1) {
    	fatal_error("Error changing stdin to non-blocking", NULL, 1);
    }

    while (true) {
    	// if reporting, don't wait to read stdin, print immediately
        if (report) {
            struct tm* t = printTime();
            int value = mraa_aio_read(tempSensor);
            float tempValue = getTemperature(value);
            printf("%.1f\n", tempValue);
            if (logging) {
                if (fprintf(logFile, "%02d:%02d:%02d %.1f\n", t->tm_hour, t->tm_min, t->tm_sec, tempValue) < 0) {
                	fatal_error("Error writing to logfile", NULL, 1);
                } 
            }
            sleep(period);
        }

        // when we wake up, check if anything happend on the keyboard
        int bytes_read = read(STDIN_FILENO, buffer+bufferindex, sizeof(char)*(BUFFER_SIZE-bufferindex));
        if (bytes_read < 0) {
            int err = errno;
            if (err != EAGAIN) {
            	fatal_error("There was an issue reading from stdin", NULL, 1);
            }
        }

        if (bytes_read > 0) {
            bufferindex += bytes_read;
            int i;
            for (i = 0; i < bufferindex; i++) {
                if (buffer[i] == '\n') {
                    temp[tempindex] = '\0';
                    char* pposition = strstr(temp, "PERIOD=");
                    if (strcmp(temp, "SCALE=F") == 0) {
                        scale = F;
                    } else if (strcmp(temp, "SCALE=C") == 0) {
                        scale = C;
                    } else if (strcmp(temp, "OFF") == 0) {
                        if (logging) {
                            if (fprintf(logFile, "%s\n", temp) < 0) {
                            	fatal_error("Error writing to logfile", NULL, 1);
                            } 
                        }
                        shutDown();
                    } else if (strcmp(temp, "STOP") == 0) {
                        report = false;
                    } else if (strcmp(temp, "START") == 0) {
                        report = true;
                    } else if (pposition != NULL && pposition == temp && strlen(temp) > 7) {
                        int num = atoi(pposition+7);
                        if (num >= 1) {
                            period = num;
                        }
                    }

                    if (logging) {
                        if (fprintf(logFile, "%s\n", temp) < 0) {
                        	fatal_error("Error writing to logfile", NULL, 1);
                        } 
                    }

                    tempindex = 0;
                } else {
                    temp[tempindex++] = buffer[i];
                }
        }

            bufferindex = 0;
        }
    }

    #ifndef DUMMY
    mraa_gpio_close(button);
    #endif
}

#endif