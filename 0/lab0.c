// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

/* Questions for TA:
 * 1. If main program doesn't have the required amount of argurments,
 * what exit() code should we use? 5...
 *
 * 2. To setup a sighandler, is it okay to use trap()?
 *  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

//TODO: write usage()
//TODO: Use error.h: error(...) for error printing.
//TODO: use re-entrant functions.
//TODO: incorporate line numbers.

const char* program_name = NULL;

void
usage() {

}

void
set_program_name(const char* argv0) {
  if(argv0 == NULL) {
    fprintf(stderr, "%s: set_program_name: %s.\n",	\
	    program_name, strerror(errno));
    abort();
  }

  const char* slash = strrchr(argv0, '/');
  const char* base = (slash != NULL ? slash + 1 : argv0);

  argv0 = base;

  program_name = argv0;
}

static struct option const long_opts[] = {
  {"input", required_argument, NULL, 'i'},
  {"output", required_argument, NULL, 'o'},
  {"segfault", no_argument, NULL, 's'},
  {"catch", no_argument, NULL, 'c'},
  //	{"help", no_argument, NULL, 'h'},
  {NULL, 0, NULL, 0}
};

void
sigseg_handler(int signum){
  fprintf(stderr, "%s [--catch]: %s\n", program_name, strerror(signum));
  exit(4);
}

//TODO: add --test -t option... set flag for universal testing

int
main(int argc, char* argv[]) {
    
  set_program_name(argv[0]);
  
  //TODO: Modularize this and put into library function. Note: use structs
  /* if(argc){ */
  /* case 1: */
  /*   fprintf(stderr, "%s: Missing argument\n", program_name); */
  /*     //usage(); */
  /*   exit(0); */
  /* case 2: */
  /*   if(argv[1][0] != '-'){ */
  /*     fprintf(stderr, "%s: %s: Unrecognized argument\n", program_name, argv[1]); */
  /*     //usage(); */
  /*     exit(1); */
  /*   } */
  /* default: */
  /*   break; */
  /* } */
  
  char* input_file = NULL;
  char* output_file = NULL;
  
  int sflag = 0,
    cflag = 0;

  // TODO: support "-" to process all optargs, record all errors, then print out errors
  int opt;
  int optind;
  while( (opt = getopt_long(argc, argv, "+i:o:sc", long_opts, &optind)) != -1) {
    switch(opt){
    case 'i':
      input_file = optarg;
      break;
    case 'o':
      output_file = optarg;
      break;
    case 's':
      sflag = 1;
      break;
    case 'c':
      cflag = 1;
      break;
    case '?':
    default:
      // Check for unrecognized arguement: print usage
      // report error and exit
      exit(1);
      break;
    }
  }

  int fd;
  if(input_file) {
    if((fd = open(input_file, O_RDONLY)) == -1) {
      fprintf(stderr,"%s [--input]: %s: %s\n", \
	      program_name, input_file, strerror(errno));
      exit(2);
    }
    close(0);
    dup(fd);
    close(fd);
  }

  if(output_file) {
    fd = creat(output_file, 0666);
    if(fd < 0) {
      fprintf(stderr,"%s [--output]: %s: %s\n",	\
	      program_name, output_file, strerror(errno));
      exit(3);
    }
    close(1);
    dup(fd);
    close(fd);
  }

  if(cflag ==1)
    // TODO: Look into trap()
    signal(SIGSEGV, sigseg_handler);
    
  if(sflag == 1) {
    char* p = NULL;
    *p = 'p';
  }
    
  char *buf[1];
  while(read(0, buf, sizeof(char)) > 0)
    write(1, buf, sizeof(char));
    
  exit(0); // successful 
}
