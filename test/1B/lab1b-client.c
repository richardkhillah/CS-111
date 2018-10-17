#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <poll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "common.h"

int TIMEOUT = 0;
const int BUF_SIZE = 256;
int mode;

/* modes are well defined:
 * mode = 3 : package[2] = {encrypt key, logfile};
 * mode = 2 : package[2] = {encrypt key, NULL};
 * mode = 1 : package[2] = {logfile, NULL};
 * mode = 0 : package[2] = {NULL, NULL}x;
 */
void set_mode(const struct options* opts, char* package[2]){

    if(opts ->encrypt.flag && opts ->log.flag) {
	mode = 3;
	char* c1 = opts->encrypt.arg;
	char* c2 = opts->log.arg;
	if( c1 && c1 ){
	    package[0] = c1;
	    package[1] = c2;
	}
	else err("unable to acces encypt or log file");
    } else if(opts ->encrypt.flag) {
        mode = 2;
	char* c = opts->encrypt.arg;
	if( c ){
	    package[0] = c;
	    package[1] = NULL;
	}
	else err("unable to access encrypt file");
    } else if(opts ->log.flag) {
	mode = 1;
	char* c = opts->log.arg;
	if( c ){
	    package[0] = c;
	    package[1] = NULL;
	}
	else err("unable to access log file");
    } else {
	package[0] = NULL;
	package[1] = NULL;
    }
}

int main(int argc, char* argv[]) {
    set_keyboard(CLIENT);

    if(argc < 2)
	err("Incorrect arguements.");
    
    struct options* opts;
    opts = get_options(argc, argv, CLIENT);

    char* package[2];
    set_mode((const struct options*)opts, package);
    
    int sockfd, portno;

    char* buffer[BUF_SIZE];
    struct sockaddr_in serv_addr;
    struct hostent* server;

    if((portno = atoi(opts-> port.arg)) < 0)
	err("Not able to convert client portno.");

    /* Create the socket */
    if( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 )
	err("Error opening socket.");
    
    server = gethostbyname("localhost");
    if(server == NULL)
	err("Client failed to get host by name.");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_UNIX;

    bcopy((char *)server->h_addr,
	  (char *)&serv_addr.sin_addr.s_addr,
	  server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	err("Client unable to connect with server");

    /* poll the socket and stdin since shell can return any time and
     * the user can input any time. */
    struct pollfd pollfds[2];
    pollfds[0].fd = STDIN_FILENO;    /* Keyboard (stdin) */
    pollfds[0].events = POLLIN;
    pollfds[0].revents = 0;
    pollfds[1].fd = sockfd;         /* input from socket */
    pollfds[1].events = POLLIN | POLLHUP | POLLERR;
    pollfds[1].revents = 0;

    while(1) {

	int poll_result = poll(pollfds, 2, TIMEOUT);
	if( poll_result  == -1)
	    err("error polling");
	if( poll_result == 0)
	    continue;
      
	/* block socket input, read keyboard, echo to stdout
	 * and forward to socket.
	 * */
	if(pollfds[0].revents & POLLIN) {
	    readwrite(STDIN_FILENO, sockfd, mode, package);
	}
    }
    
    err("MADEIT!!");
    return 0;
}
