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
#include <poll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

struct termios saved_attributes;
const char* program_name;
static struct option const long_opts[] = {
    {"port", required_argument, NULL, 'p'},
    {"log", optional_argument, NULL, 'l'},
    {"encrypt", optional_argument, NULL, 'e'},
    {"debug", optional_argument, NULL, 'd'},
    {NULL, 0, NULL, 0}
};

int port_flag = 0;
int log_flag = 0;
int encrypt_flag = 0;
int debug_flag = 0;
int host_flag = 0;

int port;
char* logfile;
char* encryptionkey;
char* host;

int BUF_SIZE = 256;
int TIMEOUT = 0;

void err(char* msg) {
    fprintf(stderr, "%s\n", msg);
}

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

void readstd(int sockfd) {
    // read standard input
    char buf[BUF_SIZE];
    FILE* logstream;
    int bytes = read(STDIN_FILENO, buf, BUF_SIZE);
    if( bytes < 0 ) err("unable to read standard input");
    
    if(log_flag) {
	logstream = fopen(logfile, "a");
	if( logstream == NULL ) err("Unable to open logfile");
    }

    /* Write read buffer byte-by-byte to appropiate spots */
    int i = 0;
    for(; i < bytes; i++) {
	char c = buf[i];
	write(STDOUT_FILENO, &buf, 1);  // write( disp )
	
	if(encrypt_flag) {              // if incrypting
	    // encrypt
	}
	
	write(sockfd, &c, 1);           // write( sock )
	if(log_flag) {                  // if logging, write( log )
	    fwrite(&c, sizeof(char), 1, logstream);
	}
    } // end loop
    if(logstream) {
	fclose(logstream);
    }
}

void readsoc(int sockfd){
    /* read socket */
    char buf[BUF_SIZE];
    FILE* logstream;
    int bytes = read(sockfd, buf, BUF_SIZE); //or BUF_SIZE-1?
    if(bytes < 0) err("Bad soc read");

    if( log_flag ) {
	logstream = fopen(logfile, "a");
	if(logstream == NULL) err("Failed open of logfile");
    }

    // main function loop
    int i = 0;
    for(; i < bytes; i++) {
	char c = buf[i];
	// if logging( log )
	if( log_flag ){
	    fwrite(&c, sizeof(char), 1, logstream);
	}
	if( encrypt_flag ) {
	    // decrypt buffer
	}
	// write(disp)
	write(STDOUT_FILENO, &c, sizeof(char));    
    }// end loop

    if( log_flag ) {
	fclose(logstream);
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
		port = atoi(optarg);
		break;
	    case 'l':
		log_flag = 1;
		logfile = optarg;
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
    set_program_name(argv[0]);
    get_options(argc, argv);

    /* Ensure --port=port option passed in */
    if(!port_flag) {
	fprintf(stderr, "Usage: --port=port\n");
    } else if( port < 1025 ) {
	fprintf(stderr, "Error: port must be greater than 1024\n");
    }
	
    /* set up socket */
    int sockfd;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    if(port < 1025) fprintf(stderr, "port must be greater than 1024");

    /* generate a socket*/
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) err("ERROR opening socket");
    
    /* get the host */
    server = gethostbyname("localhost");
    if (server == NULL) err("ERROR, no such host");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_UNIX;
    bcopy((char *)server->h_addr, 
	  (char *)&serv_addr.sin_addr.s_addr,
	  server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
	err("ERROR connecting");

    /* Begin polling service */
    struct pollfd pollfds[2];
    pollfds[0].fd = STDIN_FILENO;                   // Poll(2) stdin
    pollfds[0].events = POLLIN;                     
    pollfds[0].revents = 0;
    pollfds[1].fd = sockfd;                         // Poll(2) socket
    pollfds[1].events = POLLIN | POLLHUP | POLLERR;
    pollfds[1].revents = 0;

    while(1) {
	/* Poll(2) fd's */
	int poll_result = poll(pollfds, 2, TIMEOUT);
	if( poll_result  == -1) err("Error: Bad polling.");
	if( poll_result == 0) continue;
      
	/* block socket input and read input from keyboard */
	if(pollfds[0].revents & POLLIN) readstd(sockfd);
      
	/* block keyboard input and read output from socket */
	if(pollfds[1].revents & POLLIN) readsoc(sockfd);

	/* Something happend, so process remaining work then exit */
	if(pollfds[1].revents & (POLLHUP | POLLERR)) {
	    
	    if(debug_flag) debug("POLLHUP | POLLERR received");
	}
	break;
    }
    exit(0);
}
