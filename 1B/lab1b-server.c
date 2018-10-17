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

const char* program_name;
static struct option const long_opts[] = {
    {"port", required_argument, NULL, 'p'},
    {"encrypt", optional_argument, NULL, 'e'},
    {"debug", optional_argument, NULL, 'd'},
    {NULL, 0, NULL, 0}
};

int childpid;
int pipe2child[2];
int pipe2parent[2];

int port_flag = 0;
int encrypt_flag = 0;
int debug_flag = 0;
int host_flag = 0;

int port;
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
    // sigpipe handling
    exit(1);
}

void debug(char* msg) {
    fprintf(stderr, msg);
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

void readsoc() {
    
}

void readpipe() {

}

void shell_proc(int readpipe[2], int writepipe[2]) {
    if(childpid == 0){
	close(readpipe[1]);
	close(writepipe[0]);

	dup2(readpipe[0], STDIN_FILENO);
	dup2(writepipe[1], STDOUT_FILENO);
	dup2(writepipe[1], STDERR_FILENO);

	close(readpipe[0]);
	close(writepipe[1]);

	if(execl("/bin/bash", "bash", (char*) NULL) < 0)
	    handle_error("unable to execl");
    }
}

void init_pipes() {
    if(pipe(pipe2child) < 0) 
	handle_error("Error initializing pipe2child");
    if(pipe(pipe2parent) < 0)
	handle_error("Error initializing pipe2parent");
}

int main(int argc, char* argv[]) {
    set_program_name(argv[0]);
    get_options(argc, argv);
    
    // setup signal handlers
        
    int sockfd, newsockfd, clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) handle_error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_UNIX;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
	     sizeof(serv_addr)) < 0) err("ERROR on binding");

    // listen on socket     
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // accept connection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0) err("ERROR on accept");

    // setup pipes globally
    init_pipes();
    // fork child, redirect stdin/out/err then call exec
    close(pipe2child[0]);
    close(pipe2parent[1]);

    // breakdown undeeded pipes in parent

    // setup polling
    // poll socket and pipes

    // forward data receive from socket through pipe to shell
    // perform mappings from socket to shell
    //   ^C -> SIGINT
    //   ^D -> close write side of pipe to shell
    
    // forward data received from pipe from shell to socket
    //   EOF || SIGPIPE from shell
    //        ->harvest shell complettion status, log to stderr, exit(0)
    //   EOF || read err from network cxn (client exits), close write pipe to shell
    //        - wait fro EOR from pipe from shell, then harvest and report term stats

    // server encounters unrecognized argument, print informative usage message on stderr
    //     exit(1)
    
    return 0;
}
