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
#include <poll.h>

struct termios saved_attributes;
const char* program_name = NULL;
const int TIMEOUT = 0;
const unsigned short RBUF_SIZE = 256;
const unsigned short WBUF_SIZE = 512;
const char CR = '\r';
const char CRLF[] = {'\r', '\n'};

// global for now...
//TODO: localize these guys.
int rc_flag = 0;
pid_t rc;            /* child process */

//void Fork();
void Error(void);
void Errorm(char* func);

void process_keyboard_input(int write_fd);
void init_pipe(int pipe[]);
void reset_input_mode(void);
void set_input_mode(void);
void set_program_name(const char*);
void set_options(int argv, char* args[]);
void rw_input(void);


/* void Fork() { */
/*   pid_t rc = fork(); */
/*   if( rc < 0 ) */
/*     Error(); */
/* } */

void Error(void) {
  fprintf(stderr, "%s: %s\n", program_name, strerror(errno));
  exit(1);
}

void Errorm(char* func) {
  fprintf(stderr, "%s: %s: %s\n", program_name, func, strerror(errno));
  exit(1);
}

void process_keyboard_input(int write_fd) {
  /* read from stdin, write to stdout and forward (write) to shell */
  char rbuf[RBUF_SIZE];

  int rb_size;
  while(1) {
    rb_size = read(STDIN_FILENO, rbuf, sizeof(char)*RBUF_SIZE);
    if(rb_size < 0)
      Error();

    int rb_i;
    for(rb_i = 0; rb_i < rb_size; rb_i++) {
      char c = rbuf[rb_i];
      switch(c) {
      case 0x04: // ^D
	/* close write_fd */
	// close(write_fd);
	exit(0);
      case '\r':
      case '\n':
	if(write(STDOUT_FILENO, &CRLF, sizeof(char)*2) < 0)
	    Error();
	if( write(write_fd, &CR, sizeof(char)) < 0)
	  Error();
	break;
	/* wbuf[wb_size++] = '\r'; */
	/* wbuf[wb_size++] = '\n'; */
	break;
      default:
       	/* wbuf[wb_size++] = c; // fixthis */
	break;
      }
    }
  }
}

void init_pipe(int pipe[]){

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

void set_options(int argc, char* argv[]){
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
  char rbuf[RBUF_SIZE];
  char wbuf[WBUF_SIZE];

  //TODO: eliminate wbuf
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
  set_options(argc, argv);

  if(rc_flag == 1) {
    /* init pipes */
    int rcin_pipe[2];
    int rcout_pipe[2];
    init_pipe(rcin_pipe);
    init_pipe(rcout_pipe);

    /* fork rc the run shell */
    // run_shell();

    /* Widow unused ends of pipes */
    // rcin_pipe[0]
    // rcout_pipe[1]

    /* Setup terminal polling service */
    struct pollfd pollfds[2];
    pollfds[0].fd = 0;
    pollfds[0].events = POLLIN;
    pollfds[0].revents = 0;
    pollfds[1].fd = rcout_pipe[0];
    pollfds[1].events = POLLIN | POLLHUP | POLLERR;
    pollfds[1].revents = 0;
     
    /*TODO: Use set_pollfds
     * set_pollfds(&pollfds);
     */
    
    /* run main loop */
    while (1) {
      /* Poll terminal inputs */
      if(poll(pollfds, 2, TIMEOUT) == -1)
	Error();

      /* block shell input and read input from keyboard */
      if(pollfds[0].revents & POLLIN) {
	/* handle input from keyboard */
      }

      if(pollfds[1].revents & POLLIN) {
	/* handle input from shell */
      }

      if(pollfds[1].revents & (POLLHUP + POLLERR)) {
	/* kill shell */
      }
    }

    
  } else {

  }
  
  while(1) {
    rw_input();
  }
  return 0;
}
