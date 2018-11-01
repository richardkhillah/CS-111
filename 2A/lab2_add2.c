#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>


#define THREADS_FLAG 't'
#define ITERATIONS_FLAG 'i'
#define YIELD_FLAG 'y'
#define SYNC_FLAG 's'

#define NONE 'n'
#define MUTEX 'm'
#define SPIN 's'
#define CAS 'c'


const char USAGE[] = "Usage: ./lab2_add [threads number] [iterations number] [yield] [sync type]\n"; 


int numThreads = -1;
int numInterations = -1;
bool yield = false;
bool shouldSync = false;
char syncType = NONE;

long long counter = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int spinLock = 0;

void sigHandler(int sig) {
    if (sig == SIGSEGV) {
        fprintf(stderr, "Segmentation fault occured");
        exit(2);
    }
}

void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    if (yield) {
        sched_yield();
    }
    *pointer = sum;
}

void cas_add(long long *pointer, long long value) {
    long long temp;
    long long new;
    do {
        temp = *pointer;
        new = temp + value;
        if (yield) {
            sched_yield();
        }  
    } while(__sync_val_compare_and_swap(pointer, temp, new) != temp);
}

void *threadFunc() {
    for (int i = 0; i < numInterations; i++) {
        switch (syncType) {
            case NONE: {
                add(&counter, 1);
                break;
            }
            case MUTEX: {
                if (pthread_mutex_lock(&lock) != 0) {
                    printf("lab2: there was a problem grabbing the lock\n");
                    exit(2);
                }
                add(&counter, 1);
                if (pthread_mutex_unlock(&lock) != 0) {
                    printf("lab2: there was a problem releasing the lock\n");
                    exit(2);
                }
                break;
            }
            case SPIN: {
                while(__sync_lock_test_and_set(&spinLock, 1));


                add(&counter, 1);

                __sync_lock_release(&spinLock);
                break;
            }
            case CAS: {
                cas_add(&counter, 1);
                break;
            }
        }
    }

    for (int i = 0; i < numInterations; i++) {
        switch (syncType) {
            case NONE: {
                add(&counter, -1);
                break;
            }
            case MUTEX: {
                if (pthread_mutex_lock(&lock) != 0) {
                    printf("lab2: there was a problem grabbing the lock\n");
                    exit(2);
                }
                add(&counter, -1);
                if (pthread_mutex_unlock(&lock) != 0) {
                    printf("lab2: there was a problem releasing the lock\n");
                    exit(2);
                }
                break;
            }
            case SPIN: {
                while(__sync_lock_test_and_set(&spinLock, 1));

                add(&counter, -1);
                
                __sync_lock_release(&spinLock);
                break;
            }
            case CAS: {
                cas_add(&counter, -1);
                break;
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int currentIndex = 1;
    char opt;

    signal(SIGSEGV, sigHandler);

    // white list args and map strings to chars
    static struct option allowedOpts[] = {
        {"threads", required_argument, NULL, THREADS_FLAG},
        {"iterations", required_argument, NULL, ITERATIONS_FLAG},
        {"yield", no_argument, NULL, YIELD_FLAG},
        {"sync", required_argument, NULL, SYNC_FLAG},
        {0, 0, 0, 0}
    };

    // parse options
    while ((opt = getopt_long(argc, argv, "yt:i:s:", allowedOpts, &currentIndex)) != -1) {
        switch (opt) {
            case THREADS_FLAG:
                if (optarg) {
                    numThreads = atoi(optarg);

                    if (numThreads <= 0) {
                        fprintf(stderr, "lab2: --threads: Threads must be a number greater than 0\n%s", USAGE);
                        exit(1);
                    }
                }
                break;
            case ITERATIONS_FLAG:
                if (optarg) {
                    if (optarg) {
                        numInterations = atoi(optarg);
    
                        if (numInterations <= 0) {
                            fprintf(stderr, "lab2: --iterations: iterations must be a number greater than 0\n%s", USAGE);
                            exit(1);
                        }
                    }
                    break;
                }
                break;
            case YIELD_FLAG:
                yield = true;
                break;
            case SYNC_FLAG:
                shouldSync = true;
                if (optarg) {
                    switch (*optarg) {
                        case MUTEX:
                            syncType = MUTEX;
                            break;
                        case SPIN:
                            syncType = SPIN;
                            break;
                        case CAS:
                            syncType = CAS;
                            break;
                        default:
                            fprintf(stderr, "lab2: Invalid sync type, options are m, s, c \n%s", USAGE);
                            exit(1);
                    }
                }
                break;
            case '?':
            default:
                fprintf(stderr, "lab2: Invalid flag or argument needed\n%s", USAGE);
                exit(1);
        }
    }

    if (numThreads <= 0) {
        numThreads = 1;
    }

    if (numInterations <= 0) {
        numInterations = 1;
    }

    printf("add-");
    if (yield) {
        printf("yield-");
    }

    if (syncType == NONE) {
        printf("none,");
    } else {
        printf("%c,", syncType);
    }

    pthread_t *threads;
    threads = (pthread_t*)malloc(numThreads * sizeof(pthread_t));

    if (threads == NULL) {
        fprintf(stderr, "lab2: problem initing threads\n");
        exit(2);
    }

    struct timespec startTime, endTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 0; i < numThreads; i++) {
        if (pthread_create(threads + i, NULL, threadFunc, NULL) != 0) {
            fprintf(stderr, "lab2: Problem initing threads\n");
            exit(2);
        }
    }

    for (int i = 0; i < numThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Problem joining threads\n");
            exit(2);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    long long elapsedtime = (endTime.tv_sec - startTime.tv_sec) * 1000000000;
    elapsedtime += endTime.tv_nsec;
    elapsedtime -= startTime.tv_nsec;

    long long numOperations = numThreads*numInterations*2;
    long long averageTime = elapsedtime / numOperations;

    
    printf(", %d,%d,%lld,%lld,%lld,%lld\n", numThreads, numInterations, numOperations, elapsedtime, averageTime, counter);

    free(threads);

    return EXIT_SUCCESS;
}