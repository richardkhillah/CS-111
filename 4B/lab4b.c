// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

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
