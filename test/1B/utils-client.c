#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "common.h"

int BUF_SIZE = 256;


//mode 3: encrypt && log
//mode 2: encrypt
//mode 1: log
//mode 0: normal
void readwrite(int readfd, int writefd, int mode, char* package[2]) {
    if(mode == 3 || mode == 1){
	//open a file stream for package[1]//
    }
    char* buf[BUF_SIZE];
    int bytesread = read(readfd, buf, BUF_SIZE);
    if (bytesread < 0) err("error reading readfd");

    int i = 0;
    for(; i < bytesread; i++){
	char c = buf[i];
	switch(c){
	case '\r':
	case '\n':
	    write(STDOUT_FILENO, "\r\n", 2);
	    if(mode == 2 
	}// end switch
    }// end for
    
    
}
