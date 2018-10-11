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
#include <signal.h>
#include <poll.h>

struct termios saved_attributes;
const char* program_name = NULL;

static struct option const long_opts[] = {
  {"shell", no_argument, NULL, 's'},
  {NULL, 0, NULL, 0}
};

const int TIMEOUT = 0;
const unsigned short RBUF_SIZE = 256;
const unsigned short WBUF_SIZE = 512;
const char CR = '\r';
const char LF = '\n';
const char CRLF[] = {'\r', '\n'};

// global for now...
//TODO: localize these guys.
int rc_flag = 0;
pid_t rc;            /* child process */

void Error(void);
void run_shell(int pipe2shell[2], int pipe2term[2]);
void process_shell_output(int read_fd);
int process_keyboard_input(int pipe2shell, int pipe2term);
void init_pipe(int pipe[]);
void reset_input_mode(void);
void set_input_mode(void);
void set_program_name(const char*);
void set_options(int argv, char* args[]);
void rw_input(void);

void process_shell_output(int read_fd) {
  char rbuf[RBUF_SIZE];
  int rb_size = read(read_fd, rbuf, RBUF_SIZE);
  if(rb_size < 0)
    Error();

  int rb_i;
  for(rb_i = 0; rb_i < rb_size; rb_i++) {
    char c = rbuf[rb_i];
      switch (c) {
      case 0x04: // ^D == EOF	
	exit(0);
      case '\r':
      case '\n':
	if(write(STDOUT_FILENO, &LF, sizeof(char)*2) < 0)
	  Error();
	break;
      default:
	if(write(STDOUT_FILENO, &c, sizeof(char)) < 0)
	  Error();
	break;
      }
  }
}

int process_keyboard_input(int pipe2shell, int pipe2term) {
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
      case 0x03:// ^C : interrupt character
	kill(rc, SIGINT);
	//	exit(1);
      case 0x04: // ^D : EOF
	close(pipe2shell);
	return 0x04; // no more keyboard input
      case '\r':
      case '\n':
	if(write(STDOUT_FILENO, &CRLF, sizeof(char)*2) < 0)
	    Error();
	if(write(pipe2shell, &CR, sizeof(char)) < 0)
	  Error();
	break;
      default:
	if(write(STDOUT_FILENO, &c, sizeof(char)) < 0)
	  Error();
	if(write(pipe2shell, &c, sizeof(char)) < 0)
	  Error();
	break;
      }
    }
  }
}




int main(int argc, char* argv[]) {
  set_input_mode();
  set_program_name(argv[0]);
  set_options(argc, argv);

  if(rc_flag == 1) {
    int pipe2shell[2];
    int pipe2term[2];
    init_pipe(pipe2shell);
    init_pipe(pipe2term);

    /* fork rc the run shell */
    run_shell(pipe2shell, pipe2term);
    
    /* Widow unused ends of pipes */
    close(pipe2shell[0]);
    close(pipe2term[1]);

    /* Setup terminal polling service */
    struct pollfd pollfds[2];
    pollfds[0].fd = 0;
    pollfds[0].events = POLLIN;
    pollfds[0].revents = 0;
    pollfds[1].fd = pipe2term[0];
    pollfds[1].events = POLLIN | POLLHUP | POLLERR;
    pollfds[1].revents = 0;
        
    /* run main loop */
    while (1) {
      /* Poll terminal inputs */
      if(poll(pollfds, 2, TIMEOUT) == -1)
    	Error();

      /* block shell input and read input from keyboard */
      if(pollfds[0].revents & POLLIN) {
    	/* handle input from keyboard */
	int ret = process_keyboard_input(pipe2shell[1]);
	switch(ret) {
	case 0x03:// ^C : interrupt character
	  continue;
	case 0x04: // ^D : EOF
	  // close(pipe2shell);
	default:
	  break;
	
	}
      }

      if(pollfds[1].revents & POLLIN) {
    	/* handle input from shell */
	process_shell_output(pipe2term[0]);
      }

      if(pollfds[1].revents & (POLLHUP + POLLERR)) {
    	/* kill shell */
      }
    } // end while
  } else {
    while(1) {
      rw_input();
    }
  }
  
  return 0;
} // end main

void Error(void) {
  fprintf(stderr, "%s: %s\n", program_name, strerror(errno));
  exit(1);
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

  while(1) {
    int rb_size = read(STDIN_FILENO, rbuf, RBUF_SIZE);
    if (rb_size < 0)
      Error();

    /* process only the amount of bytes that were read into rbuf */
    int rb_i;
    for(rb_i = 0; rb_i < rb_size; rb_i++){
      char c = rbuf[rb_i];
      switch (c) {
      case 0x04: // ^D
	exit(0);
      case '\r':
      case '\n':
	if(write(STDOUT_FILENO, &CRLF, sizeof(char)*2) < 0)
	  Error();
	break;
      default:
	if(write(STDOUT_FILENO, &c, sizeof(char)) < 0)
	  Error();
	break;
      }
    }
  }
}

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

void init_pipe(int pipefd[2]){
  if(pipe(pipefd) < 0)
    Error();
}

void run_shell(int pipe2shell[2], int pipe2term[2]) {
  rc = fork();
  if(rc < 0)
    Error();

  if(rc == 0) {
    fprintf(stderr, "Hello from shell\n");
    
    /* setup shell i/o envrionment before calling execvp */
    /* widow unused pipe ends */
    close(pipe2shell[1]);  /* inputs write end */
    close(pipe2term[0]); /* outputs read end */

    /* redirect rc's stdin, stdout, stderr */
    dup2(pipe2shell[0], 0);
    dup2(pipe2term[1], 1);
    dup2(pipe2term[1], 2);

    /* widow redundant streams */
    close(pipe2shell[0]);
    close(pipe2term[1]);

    const char* file = "/bin/bash";
    char* const args[] = {NULL};
    if(execvp(file, args) < 0)
      Error();
  }
}
