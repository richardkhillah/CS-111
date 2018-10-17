#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.h"

int main(int argc, char* argv[]){
    struct options* opts;
    opts = get_options(argc, argv, CLIENT);

    /* set up socket */
    int sockfd, newsockfd, portno, clilen, bytesread;

    struct sockadd_in serv_addr, cli_addr;

    /* create socket */
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd < 0)
	error("server unable to create socket");

    bzero((char*)&serv_addr, sizeof(serv_addr));
    if((portno = atoi(opts->port.arg)) < 0)
	error("Unable to assign portno");

    /* configure server socket */
    serv_addr.sin_family = AF_UNIX;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* bind the socket to the servers address */
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	error("Error binding server socket to server address");

    /* wait for a client to connect */
    listen(sockfd, 5);

    //process io from client
    return 0;
}
