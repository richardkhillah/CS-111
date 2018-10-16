// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// UID: 604853262

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <getopt.h>
#include <termios.h>

struct termios saved_attributes;

static struct option const long_opts[] = {
    {"port", required_argument, NULL, 'p'},
    {"log", optional_argument, NULL, 'l'},
    {"encrypt", optional_argument, NULL, 'e'},
    {"debug", optional_argument, NULL, 'd'},
    {NULL, 0, NULL, 0}
};

int log_flag = 0;
int encrypt_flag = 0;
int debug_flag = 0;

int portno;
char* logfile;
int lffd;                // logfile fd

void handle_error(char* msg) {
    fprintf(stderr, "[%s]: %s (errno %d)\n", msg, strerror(errno), errno);
    exit(1);
}

void handle_failed_syscall(int signum) {
    // print message
    exit(1);
}

void debug(char* msg) {
    fprintf(stderr, msg);
}

/* read_write will read from either stdin, or a network socket
 * sel(ect) detmines whether we are reading from the keyboard
 * and writing to the server (select 0) or reading from the
 * server and writing to the terminal (select 1). */
void read_write(int readfd, int writefd, int sel) {
    char* buf[BUF_SIZE];
    int bytes_read = read(read_fd, buf, BUF_SIZE);
    if(bytes_read < 0) handle_error("Error reading readfd");

    if(log_flag) {
	switch(sel){
	case 0:
	    fprintf(lffd, "SENT %d bytes: ", bytes_read);
	    break;
	case 1:
	    fprintf(lffd, "RECEIVED %d bytes: ", bytes_read);
	    break;
	default:
	    handle_error("Invalid read_write select.");
	    exit(1);
	}
	write(lffd, buf, bytes_read);
	write(lffd, "\n", 1);
    }
    
    int i = 0;
    for(; i < bytes_read; i++){
	char c = buff[i];
	// process char by char and echo to stdout
	if (encrypt_flag == 1 && sel == 1) {
	    // decrypt input from server
	}
	write(STDOUT_FILENO, &c, sizeof(char));
	
	// if encrypt_flag = 1 and we're reading from server,
	// decrypt before writing to stdout 
	// write to write_fd
    }
}

void get_options(int argc, char* argv[]){
    int opt;
    int optind;
    while((opt = getopt_long(argc, (char* const*)argv, \
			     "sd", long_opts, &optind)) != -1)
	{
	    switch (opt) {
	    case 'p':
		portno = atoi(optarg);
		break;
	    case 'l':
		log_flag = 1;
		logfilename = optarg;
		int status = open(lffd, logfile, O_APPEND);
		if( status < 0) handle_error("unable to open file");
		break;
	    case 'e':
		encrypt_flag = 1;
		break;
	    case 'd':
		debug_flag = 1;
		break;
	    default:
		// print custom error
		break;
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

int main(int argc, char* argv[]) {
    set_input_mode();
    // set_program_name(argv[0]);
    get_options(argc, argv);

    // set up socket

    // set up polling service
    // listen at stdin and server socket (once connection established)
    
    // read input from keyboard and echo to display
    // and forward to socket

    // if log_flag, write to log file
    // if encrypt_flag, encrypt then forward to socket and logfile

    // read input from socket and write to display
    // if log_flag, write to log file
    // if encrypt_flag, decrypt then write to display and log file.
    
    return 0;
}
