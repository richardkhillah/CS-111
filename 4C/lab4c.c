// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#ifndef DUMMY
#include <mraa/aio.h>
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

// lab4C unique headers
#include <netdb.h>
#include <netinet/in.h>     // so we can use sockets
#include <openssl/ssl.h>    // so we can use OpenSSL
#include <openssl/err.h>    // so we can use OpenSSL

#define EXIT_OK 0
#define EXIT_BADARG 1
#define EXIT_OTHER 2

#define B 4275
#define R0 100000
#define F 'f'
#define C 'c'
#define PERIOD_FLAG 'p'
#define LOG_FLAG 'l'
#define SCALE_FLAG 's'

#define ID_FLAG 'i'
#define HOST_FLAG 'h'

#define BUFFER_SIZE 1024

int period = 1;
char scale = F;
bool report = true;
FILE* logFile = NULL;
bool logging = false;

// New Parameters
int id = -1;
char* host = NULL;
int port = -1;

SSL *ssl = NULL;
int server_socket = -1;


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

struct tm* printTime() {
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
        if (fprintf(logFile, "%02d:%02d:%02d SHUTDOWN\n", t->tm_hour, 
                    t->tm_min, t->tm_sec) < 0) 
        {
            fprintf(stderr, "There was a problem writing to output\n");
            exit(1);
        } 
    }
    exit(EXIT_OK);
}

void usage(void) {
	fprintf(stderr, "usage: ./%s --id=9_digit_id --host=name --log=<filename> port [--period=<seconds>] [--scale=[f, c]]\n", program_name);
    fprintf(stderr, "    id: required 9 digit ID\n");
    fprintf(stderr, "    host: required name or address\n");
    fprintf(stderr, "    log: required filename to log session\n");
    fprintf(stderr, "    port: required port number\n");
    fprintf(stderr, "    period: optional, time (in seconds) between temperature sensor readings\n");
    fprintf(stderr, "            default period is 1 second\n");
    fprintf(stderr, "    scale: optional, temperature scale in degrees fahrenheit (default) or celsius.\n");
}

void getoptions(int argc, char* const* argv) {
    // The program has four required arguments, so ensure there
    // are at least 5 arguments in the program
    if(argc < 5)
        fatal_error("Missing Arguments", (void*)usage, EXIT_BADARG);

    // Find non-switch argument and assign to port number
    for(int p = 1; p < argc; p++) {
        // A switch argument begins with a '-'
        if(argv[p][0] != '-') {
            port = atoi(argv[p]);
            if(port < 0) {
                fatal_error("Port number must be greater than 0",
                            (void*) usage, EXIT_BADARG);
            }
        }
    }

	int optind = 1;
    int opt;
    struct option long_options[] = {
        {"id", required_argument, NULL, ID_FLAG},
        {"host", required_argument, NULL, HOST_FLAG},
        {"log", required_argument, NULL, LOG_FLAG},
        {"period", required_argument, NULL, PERIOD_FLAG},
        {"scale", required_argument, NULL, SCALE_FLAG},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "p:l:s:", long_options, &optind)) != -1) {
        switch (opt) {
            case ID_FLAG: {
                // Ensure id is 9 digits in length
                if (strlen(optarg) != 9){
                    fatal_error("Incorrect ID", (void)usage, EXIT_BADARG);
                }
                id = atoi(optarg);
                if(id < 0) {
                    fatal_error("Id must be greater than zero",
                                (void*)usage, EXIT_BADARG);
                }
                break;
            }
            case HOST_FLAG: {
                host = optarg;
                if(host == NULL) {
                    fatal_error("Internal Error", NULL, -1);
                }
                break;
            }
            case PERIOD_FLAG:
                if (optarg) {
                    period = atoi(optarg);
                    if (period < 1) {
                    	fatal_error("Period must be greater-than or equal to 0", 
                                    (void*)usage, 1);
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
                    	fatal_error("unknown scale... scale must be \"F\" or \"C\"",
                                    (void*)usage, 1);
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
    // ENSURE THAT WE HAVE ALL REQUIRED ARGUMENTS (ID, HOST, LOG, PORT NUMBER)
    // BEFORE CONTINUING
    if ( id == -1 || host == NULL || logfile == NULL || port == -1 ) {
        fatal_error("required_arguments not met", (void*)usage, EXIT_BADARG);
    }

    // Open a TCP connection to the server at the specified address and port
    /* Create a socket */
    struct sockaddr_in server_address;
    struct hostent *server;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
        fatal_error("Error setting up socket", NULL, EXIT_OTHER);
    }

    /* This is the user-defined host */
    server = gethostbyname(host);
    if (server == NULL) {
        if( h_error == HOST_NOT_FOUND) {
            fatal_error("Host not found", NULL, EXIT_OTHER);
        } else if (h_error == TRY_AGAIN) {
            fatal_error("Try again", NULL, EXIT_OTHER);
        } else if(h_error == NO_RECOVERY) {
            fatal_error("Non-recoverable server failure. Trying again will NOT help",
                        NULL, EXIT_OTHER);
        } else {
            fatal_error("Unknown Error... Trying again MIGHT help (it also might not)",
                        NULL, EXIT_OTHER);
        }
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length);
    server_address.sin_port = htons(port);

    // Immediately send (and log) an ID termindated with a newline:
    //      ID=ID-number

    // Send (and log) newline terminated temperature reports over the cxn

    // process (and log) newline-terminated commands received over the
    // connection.
    // If the temperature reports are mis-formatted, the server will return
    // a LOG command with a description of the error


    mraa_aio_context tempSensor;
    tempSensor = mraa_aio_init(1);

    char buffer[BUFFER_SIZE];
    char temp[BUFFER_SIZE*2];
    int tempindex = 0;
    int bufferindex = 0;

    // set keyboard to non-block
    if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1) {
    	fatal_error("Error changing stdin to non-blocking", NULL, EXIT_OTHER);
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
                	fatal_error("Error writing to logfile", NULL, EXIT_OTHER);
                } 
            }
            sleep(period);
        }

        // when we wake up, check if anything happend on the keyboard
        int bytes_read = read(STDIN_FILENO, buffer+bufferindex, sizeof(char)*(BUFFER_SIZE-bufferindex));
        if (bytes_read < 0) {
            int err = errno;
            if (err != EAGAIN) {
            	fatal_error("There was an issue reading from stdin", NULL, EXIT_OTHER);
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
                            	fatal_error("Error writing to logfile", NULL, EXIT_OTHER);
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
                        	fatal_error("Error writing to logfile", NULL, EXIT_OTHER);
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
}
