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
#include <sys/types.h>
#include <sys/wait.h>

struct termios saved_attributes;
const char* program_name = NULL;
const int TIMEOUT = 0;
const unsigned short BUF_SIZE = 256;
//const unsigned short WBUF_SIZE = 512;
const char CR = '\r';
const char LF = '\n';
const char CRLF[] = {'\r', '\n'};

int shell_flag = 0;
int debug_flag = 0;
//char* shell_program;
pid_t shellpid;            /* child process */

int pipe2shell[2];
int pipe2term[2];


void Error(void);
void sig_handler(int signum);
void reset_input_mode(void);
void set_input_mode(void);
void set_program_name(const char*);
void set_options(int argc, char* args[]);
void rw_input(void);

void init_pipes();
void run_shell();
void print_shell_exit_status(int status);

void run_terminal(void);
void read_keyboard(void);
void read_shell(void);

int read_shell_output(int read_fd);
int read_keyboard_input(int pipe2shell);

int main(int argc, char* argv[]) {
  set_input_mode();
  set_program_name(argv[0]);
  set_options(argc, argv);

  if(shell_flag) {
    init_pipes();
    shellpid = fork();
    if(shellpid < 0) Error();

    if(shellpid == 0) 
      run_shell();
    else
      run_terminal();
  } else {
    rw_input();
  }
  
  int status;
  if(waitpid(shellpid, &status, 0) < 0)
    Error();
  print_shell_exit_status(status);
  return 0;
}

void sig_handler(int signum){
  if(shell_flag && signum == SIGINT)
    kill(shellpid, SIGINT);
  if(signum == SIGPIPE)
    exit(1);
}

void print_shell_exit_status(int status) {
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d",
	  WTERMSIG(status), WEXITSTATUS(status));

}

void read_shell(void) {
  char buf[BUF_SIZE];
  while(1) {
    int bytes_read = read(pipe2term[0], buf, BUF_SIZE);
    if (bytes_read < 0) Error();

    int i;
    for(i = 0; i < bytes_read; i++) {
      char c= buf[i];
      switch(c) {
      case 0x04:
	close(pipe2term[1]); // close shell's write pipe ie shell output
	return;
      case '\r':
      case '\n':
	write(STDOUT_FILENO, &"\r\n", sizeof(char)*2);
	break;
      default:
	write(STDOUT_FILENO, &c, sizeof(char));
	break;
      } // end switch 
    }//end for
  }//end while
}

void read_keyboard(void) {
  char buf[BUF_SIZE];
  int status = 0;
  while(1) {
    int bytes_read = read(STDIN_FILENO, buf, BUF_SIZE);
    if(bytes_read < 0) Error();

    int w_status;
    int i;
    for(i = 0; i < bytes_read; i++) {
      char c = buf[i];
      switch (c){
      case 0x04: /* ^D */
	close(pipe2shell[1]);  // close term write pipe ie shell input
	waitpid(shellpid,&status,0);	// continue to process shell output
	print_shell_exit_status(status);
	continue;
      case 0x03: /* ^C */
	kill(shellpid, SIGINT);
	//continue;
	break;
      case '\r':
      case '\n':
	write(STDOUT_FILENO, &"\r\n", sizeof(char)*2);
	w_status = write(pipe2shell[1], &"\n", sizeof(char));
	break;
      default:
	write(STDOUT_FILENO, &c, sizeof(char));
	w_status = write(pipe2shell[1], &c, sizeof(char));
	break;
      }// end switch

      if(w_status == -1) {
	if(errno & EPIPE) {
	  // handle shutdown gracefully
	}
	Error();
      }// endif
    } // end for
  } // end while
}

void run_terminal(void) {
    close(pipe2shell[0]); // because parent writes to shell
    close(pipe2term[1]); // beacuse parent reads from shell

    struct pollfd pfds[2];
    pfds[0].fd = STDIN_FILENO; // input from keyboard
    pfds[1].fd = pipe2term[0]; // input from shell

    pfds[0].events = 0;
    pfds[1].events = 0;
    pfds[0].events |= POLLIN | POLLHUP | POLLERR;
    pfds[1].events |= POLLIN | POLLHUP | POLLERR;

    pfds[0].revents = 0; // initial vals
    pfds[1].revents = 0; // set after each polling cycle.

    while(1) {
      if(poll(pfds, 2, TIMEOUT) == -1) Error();

      if(pfds[0].revents & POLLIN) {
	read_keyboard(); // only, all i/o blocked
	//	pfds[0].revents = 0;
      }
      if(pfds[1].revents & POLLIN) {
	read_shell(); // ditto read_keyboard()
	//	pfds[1].revents = 0;
      }
      if(pfds[1].revents & (POLLERR | POLLHUP)) { // poll error from shell => SIGINT
	fprintf(stderr, "pfds[1].revents = POLLERR\n");
	read_shell(); // do final read
	close(pipe2term[0]); // all shell output has been processed by this time
	break;
      }
    }
}

void run_shell(void) {
  close(pipe2shell[1]); // becuase child reads from this pipe
  close(pipe2term[0]); // because child writes to this pipe

  dup2(pipe2shell[0], 0); // make childs stdin
  dup2(pipe2term[1], 1); // make childs stdout
  dup2(pipe2term[1], 2); // make childs stderr

  close(pipe2shell[0]);
  close(pipe2term[1]);
  char* args[] = {NULL};
  //  if(execl(path, path, (char*) NULL) == -1)
  if(execvp("/bin/bash", args) < 0)
    Error();    
}

void init_pipes(void) {
  if(pipe(pipe2shell) < 0) Error();
  if(pipe(pipe2term) < 0) Error();
}

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
  static struct option const long_opts[] = {
    {"shell", optional_argument, NULL, 's'},
    {"debug", no_argument, NULL, 'd'},
    {NULL, 0, NULL, 0}
  };
  
  int opt;
  int optind;
  while((opt = getopt_long(argc, (char* const*)argv, "s::", long_opts, &optind)) != -1) {
    switch (opt) {
    case 's':
      signal(SIGINT, sig_handler);
      signal(SIGPIPE, sig_handler);
      shell_flag = 1;
      break;
    case 'd':
      debug_flag = 1;
      break;
    default:
      Error();
    }
  }
}

void rw_input(void) {
  char buf[BUF_SIZE];

  while(1) {
    int bytes_read = read(STDIN_FILENO, buf, BUF_SIZE);
    if (bytes_read < 0)
      Error();

    int i;
    for(i = 0; i < bytes_read; i++){
      char c = buf[i];
      switch (c) {
      case 0x04:
	exit(0);
      case '\r':
      case '\n':
	if(write(STDOUT_FILENO, "\r\n", sizeof(char)*2) < 0)
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
