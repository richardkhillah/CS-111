// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// UID: 604853262

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>

struct termios saved_attributes;
const char* program_name = NULL;

const unsigned short RBUF_SIZE = 256;
const unsigned short WBUF_SIZE = 512;

int rc_flag = 0;
pid_t rc;            /* child process */
int rcpipe_in[2];    /**/
int rcpipe_out[2];   /**/

void Error(void);
void Errorm(char* func);
void reset_input_mode(void);
void set_input_mode(void);
void set_program_name(const char*);
void Getopts(int argv, char* args[]);
void rw_input(void);


void Fork() {
  pid_t pid_rc = fork();
  if( pid_rc < 0 )
    Error();
}

void Error(void) {
  fprintf(stderr, "%s: %s\n", program_name, strerror(errno));
  exit(1);
}

void Errorm(char* func) {
  fprintf(stderr, "%s: %s: %s\n", program_name, func, strerror(errno));
  exit(1);
}

static struct option const long_opts[] = {
  {"shell", no_argument, NULL, 's'},
  {NULL, 0, NULL, 0}
};

void reset_input_mode(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode(void) {
  struct termios tattr;
  //  char* name;
  
  //TODO: Change error message
  if(!isatty(STDIN_FILENO)) {
    fprintf(stderr, "Not a terminal.\n");
    exit(1);
  }
  
  /* Save original attributes so they can be restored later */
  tcgetattr(STDIN_FILENO, &saved_attributes);
  atexit(reset_input_mode);

  /* Set new terminal mode */
  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_iflag = ISTRIP;   /* use only lower 7 bits */
  tattr.c_oflag = 0;        /* no processing */
  tattr.c_lflag = 0;        /* no processing */
  
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void set_program_name(const char* argv0) {
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

void Getopts(int argc, char* argv[]){
  int opt;
  int optind;
  while((opt = getopt_long(argc, (char* const*)argv, "s", long_opts, &optind)) != -1) {
    switch (opt) {
    case 's':
      //TODO: setup sighandler
      rc_flag = 1;
      break;
    default:
      Error();
    }
  }
}

void rw_input(void) {
  char* rbuf[RBUF_SIZE];
  char* wbuf[WBUF_SIZE];
  
  while(1) {
    int rb_size = read(STDIN_FILENO, rbuf, RBUF_SIZE);
    if (rb_size < 0)
      Error();

    /* process only the amount of bytes that were read into rbuf */
    int rb_i;
    int wb_size = 0;   /* wbuf might be larger than rbuf after mapping */
    for(rb_i = 0; rb_i < rb_size; rb_i++){
      char c = rbuf[rb_i];
      switch (c) {
      case 0x04: // ^D
	exit(0);
      case '\r':
      case '\n':
	wbuf[wb_size++] = '\r';
	wbuf[wb_size++] = '\n';
	//case 0x0a:
	//case 0x0d:
	break;
      default:
	wbuf[wb_size++] = c; // fixthis
	break;
      }
    }
    
    /* write buffer to screen */
    /* int bytes_written = write(STDOUT_FILENO, wbuf, wb_size+1); */
    int bytes_written;
    for(bytes_written = 0; bytes_written < wb_size; bytes_written++) {
      write(STDOUT_FILENO, &wbuf[bytes_written], sizeof(char));
    }
    
    if (bytes_written < 0) {
      fprintf(stderr, "%s: %s\n", program_name, strerror(errno));
      exit(1);
    }
  }
}

int main(int argc, char* argv[]) {
  set_input_mode();
  set_program_name(argv[0]);
  Getopts(argc, argv);

  if(rc_flag == 1) {
    Fork(); /* Make child process */
    /*   */

  } else {

  }
  
  while(1) {
    rw_input();
  }
  return 0;
}
