// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#define THREADS 't'
#define ITERATIONS 'i'
#define YIELD 'y'
#define SYNC 's'

#define NONE 'n'
#define MUTEX 'm'
#define SPIN 's'
//#define CAS 'c'

#define DEFAULT 1

#define true 1
#define false 0

#define DEBUG 'd'

typedef int bool;

int numThreads;
int numIterations;
int opt_yield;
bool sync_flag;
char sync_type;

bool debug_flag;

void usage();

void get_options(int argc, char* const* argv);

void tag();

void handle_sig(int sig);