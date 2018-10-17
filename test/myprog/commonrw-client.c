#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <fcntl.h>

#include "common.h"
#include "commonrw.h"

const int BUF_SIZE = 256;

void read_write(char* readpath, char* writepath) {
    int readfd = open(readpath, O_RDONLY);
    if(readfd == -1) {
	fprintf(stderr, "Error opening readpath: %s\n", readpath);
	exit(1);
    }

    char buf[BUF_SIZE];
    int bytes_read = read(readfd, buf, BUF_SIZE);
    if(bytes_read < 0){
	fprintf(stderr, "error reading readfd: %d assigned from readpath: %s\n",
		readfd, readpath);
	exit(1);
    }

    close(readfd);

    FILE* writestream = fopen(writepath, "a");
    if(writestream == NULL) {
	fprintf(stderr, "Error opening writepath: %s", writepath);
	exit(1);
    }

    int i = 0;
    for(; i < bytes_read; i++) {
	char c = buf[i];
	write(STDOUT_FILENO, &c, 1);
	fwrite(&c, sizeof(char), 1, writestream);
    }

    fclose(writestream);
    
    fprintf(stderr, "finished reading & writing using commonrw-client\n");
    exit(0);
}

