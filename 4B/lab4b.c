#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <mraa/gpio.h>
#include <mraa/aio.h>


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
const char USAGE[] = "Usage: ./lab4 [period number] [log filename]\n";
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

int main(int argc, char* argv[]) {
    int currentIndex = 1;
    int opt;

    // while list args and map strings to chars
    struct option allowedOpts[] = {
        {"period", required_argument, NULL, PERIOD_FLAG},
        {"log", required_argument, NULL, LOG_FLAG},
        {"scale", required_argument, NULL, SCALE_FLAG},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "p:l:s:", allowedOpts, &currentIndex)) != -1) {
        switch (opt) {
            case PERIOD_FLAG:
                if (optarg) {
                    period = atoi(optarg);
                    if (period < 1) {
                        fprintf(stderr, "lab4: Period must be >= 0\n");
                        exit(1);
                    }
                }
                break;
            case LOG_FLAG:
                if (optarg) {
                    logFile = fopen(optarg, "w");
                    if (logFile == NULL) {
                        perror("Could not open file.");
                        exit(1);
                    }
                    logging = true;
                }
                break;
            case SCALE_FLAG:
                if (optarg) {
                    optarg[0] = tolower(optarg[0]);
                    if (strcmp(optarg, "c") != 0 && strcmp(optarg, "f")) {
                        fprintf(stderr, "lab4: scale must be F or C\n");
                        exit(1);
                    }

                    scale = tolower(optarg[0]);
                }
                break;
            case '?':
            default:
                fprintf(stderr, "%s", USAGE);
                exit(1);
        }
    }

    mraa_gpio_context button;
    mraa_aio_context tempSensor;
    button = mraa_gpio_init(62);
    tempSensor = mraa_aio_init(1);

    mraa_gpio_dir(button, MRAA_GPIO_IN);

    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &shutDown, NULL);

    char buffer[BUFFER_SIZE];
    char temp[BUFFER_SIZE*2];
    int tempCursor = 0;
    int bufferCursor = 0;


    if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1) {
        perror("There was a problem changing STDIN to non blocking");
        exit(1);
    }

    while (true) {
        if (report) {
            struct tm* t = printTime();
            int value = mraa_aio_read(tempSensor);
            float tempValue = getTemperature(value);
            printf("%.1f\n", tempValue);
            if (logging) {
                if (fprintf(logFile, "%02d:%02d:%02d %.1f\n", t->tm_hour, t->tm_min, t->tm_sec, tempValue) < 0) {
                    fprintf(stderr, "There was a problem writing to output\n");
                    exit(1);
                } 
            }
            sleep(period);
        }

        int val = read(STDIN_FILENO, buffer+bufferCursor, sizeof(char)*(BUFFER_SIZE-bufferCursor));
        if (val < 0) {
            int err = errno;
            if (err != EAGAIN) {
                perror("There was a problem reading from stdin.");
                exit(1);
            }
        }

        if (val > 0) {
            bufferCursor += val;
            int i;
            for (i = 0; i < bufferCursor; i++) {
                if (buffer[i] == '\n') {
                    temp[tempCursor] = '\0';
                    char* location = strstr(temp, "PERIOD=");
                    if (strcmp(temp, "SCALE=F") == 0) {
                        scale = F;
                    } else if (strcmp(temp, "SCALE=C") == 0) {
                        scale = C;
                    } else if (strcmp(temp, "OFF") == 0) {
                        if (logging) {
                            if (fprintf(logFile, "%s\n", temp) < 0) {
                                fprintf(stderr, "There was a problem writing to output\n");
                                exit(1);
                            } 
                        }
                        shutDown();
                    } else if (strcmp(temp, "STOP") == 0) {
                        report = false;
                    } else if (strcmp(temp, "START") == 0) {
                        report = true;
                    } else if (location != NULL && location == temp && strlen(temp) > 7) {
                        int num = atoi(location+7);
                        if (num >= 1) {
                            period = num;
                        }
                    }

                    if (logging) {
                        if (fprintf(logFile, "%s\n", temp) < 0) {
                            fprintf(stderr, "There was a problem writing to output\n");
                            exit(1);
                        } 
                    }

                    tempCursor = 0;
                } else {
                    temp[tempCursor++] = buffer[i];
                }
            }

            bufferCursor = 0;
        }
    }

    mraa_gpio_close(button);
}
