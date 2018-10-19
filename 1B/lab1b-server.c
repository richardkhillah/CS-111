// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// UID: 604853262

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>
#include <getopt.h>
#include <termios.h>
#include <poll.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
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
    fprintf(stderr, "%s\r\n", msg);
}

void handle_error(char* msg) {
    fprintf(stderr, "[%s]: %s (errno %d)\r\n", msg, strerror(errno), errno);
    exit(1);
}

void handle_failed_syscall(int signum) {
    if(signum == SIGINT) {
	// child get's SIGINT
	//close pipe2parent[1]
	// exit(0)
    }

    if(signum == SIGPIPE) {
	// wait for child
	// 
    }
    // print message
    // sigpipe handling
    exit(1);
}

void debug(char* msg) {
    fprintf(stderr, "%s\r\n",msg);
}

void get_options(int argc, char* argv[]){
    int opt;
    int optind;
    while((opt = getopt_long(argc, (char* const*)argv, \
			     "sd", long_opts, &optind)) != -1)
	{
	    switch (opt) {
	    case 'p':
		port_flag = 1;
		port = atoi(optarg);
		break;
	    case 'e':
		encrypt_flag = 1;
		encryptionkey = optarg;
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

void readsoc(int sockfd) {
    // forward data receive from socket through pipe to shell
    char buf[BUF_SIZE];
    int bytes = read(sockfd, buf, BUF_SIZE-1); // TODO: BUF_SIZE-1 read??
    if(bytes < 0) handle_error("Server unsuccessful read socket");

    if(encrypt_flag) {
	// decrypt 
    }

    int i = 0;
    for(; i < bytes; i++) {
	if(debug_flag) debug("Inside server");
	char c = buf[i];
	switch(c) {
	case '\r':
	case '\n':
	    write(pipe2child[1], "\n", 1);
	    break;
	    // perform mappings from socket to shell
	case 0x03: // ^C -> SIGINT
	    if(debug_flag) debug("^C");
	    kill(childpid, SIGINT);
	    break; // do I wait here?
	case 0x04: // ^D -> close write side of pipe to shell
	    if(debug_flag) debug("^D");
	    close(pipe2child[1]);
	    break;
	    // do i wait here?
	default:
	    write(pipe2child[1], &c, 1);
	    break;
	} // end for
    }

    
}

void readpipe(int  sockfd) {
    // forward data received from pipe from shell to socket
    char buf[BUF_SIZE];
    int bytes = read(pipe2parent[0], buf, BUF_SIZE-1); // BUF_SIZE-1??
    if(bytes < 0) handle_error("Error reading shell output pipe");

    // char swap[bytes]; // pytes+1??
    if(encrypt_flag){
	// encrypt

    }

    int i = 0;
    for(; i < bytes; i++) {
	char c = buf[i];
	switch(c) {
	case 0x04: // ^D <=> EOF
	case SIGPIPE:
	    //   EOF || SIGPIPE from shell
	    //        ->harvest shell complettion status, log to stderr, exit(0)
	    // wait for shell.
	    // log stderr
	    close(sockfd);
	    exit(0);
	case '\n':
	    write(sockfd, "\r\n", 2);
	    break;
	default:
	    write(sockfd, &c, 1);
	    break;
	}
    }
}

void exec_shell() {
    if(childpid == 0){
	close(pipe2child[1]);
	close(pipe2parent[0]);

	dup2(pipe2child[0], STDIN_FILENO);
	dup2(pipe2parent[1], STDOUT_FILENO);
	dup2(pipe2parent[1], STDERR_FILENO);

	close(pipe2child[0]);
	close(pipe2parent[1]);

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
    if(!port_flag){
	fprintf(stderr, "usage: %s --port=portnumber\r\n", program_name);
	exit(1);
    } else if(port_flag && port < 1025) {
	fprintf(stderr, "Error: port must be greater than 1024\r\n");
	exit(1);
    }
    // setup signal handlers
    
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) handle_error("ERROR opening socket");
    if(debug_flag) debug("socket assigned");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
	     sizeof(serv_addr)) < 0) err("ERROR on binding");

    // listen on socket     
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // accept connection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0){
	if(debug_flag) debug("Server-side error");
	err("ERROR on accept");
    }

    // setup pipes globally
    init_pipes();
    // fork child, redirect stdin/out/err then call exec
    childpid = fork();

    if(childpid < 0)
	handle_error("Failed fork()");
    else if(childpid == 0)
	exec_shell();
    else{
	/* breakdown undeeded pipes in parent */
	close(pipe2child[0]);
	close(pipe2parent[1]);

	/* Begin polling service */
	struct pollfd pollfds[2];
	pollfds[0].fd = newsockfd;               // Poll(2) socket output
	pollfds[0].events = POLLIN;                     
	pollfds[0].revents = 0;
	pollfds[1].fd = pipe2parent[0];       // Poll(2) shell output
	pollfds[1].events = POLLIN | POLLHUP | POLLERR;
	pollfds[1].revents = 0;

	while(1) {

	    /* Poll(2) fd's */
	    int poll_result = poll(pollfds, 2, TIMEOUT);
	    if(poll_result  == -1) err("Error: Bad polling.");
	    if(poll_result == 0) continue;

	    /* block pipe input and process input from socket */
	    if(pollfds[0].revents & POLLIN){
		readsoc(newsockfd);
		pollfds[0].revents = 0;
	    }
	    /* block socket input and process input from pipe */
	    if(pollfds[1].revents & POLLIN){
		readpipe(newsockfd);
		pollfds[1].revents = 0;
	    }
	    
	    /* Something happened, process remaining work then exit */
	    if(pollfds[1].revents & (POLLHUP | POLLERR)){
		close(pipe2child[1]);
		close(newsockfd);
		int status;
		waitpid(childpid, &status, 0);
		exit(1);
		// do something
	    }

	    //   EOF || read err from network cxn (client exits), close write pipe to shell
	    //        - wait for EOF from pipe from shell, then harvest and report term stats

	    
	    // server encounters unrecognized argument, print informative usage message on stderr
	    //     exit(1)
	}
    }
    exit(0);
}
