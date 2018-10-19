#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

const int SENT = 0;             // use this approach
const int RECEIVED = 1;         // use this approach  

void fatal_error(char* my_message){
    fprintf(stderr, "%s\r\n", my_message);
    exit(1);
}

void log_entry(FILE* logstream, int type, char* buf, int len) {
    int was_written = 0;
    
    switch(type){
    case 0:
	was_written = fprintf(logstream, "SENT ");
	break;
    case 1:
	was_written = fprintf(logstream, "RECEIVED ");
	break;
    default:
	fatal_error("ERROR invalid logging() option");
    }
    was_written += fprintf(logstream, "%d bytes: %s\n", len, buf);

    if( was_written == -1 ) fatal_error("ERROR loggin transmission");
}







/* const char* writefile = "testing-logging.c"; */
/* const char* readfile = "hello.txt"; */

/* int main(void) { */
/*     FILE* readstream = fopen(readfile, "r"); */
/*     FILE* writestream = fopen(writefile, "a+"); */

/*     if (writestream == NULL || readstream == NULL){ */
/* 	fprintf(stderr, "failure to open.\n"); */
/* 	exit(1); */
/*     } */
/*     char buf[6200]; */
/*     int bytes_read; */
/*     bytes_read = fread(buf, sizeof(char), 6200, readstream); */
/*     if( bytes_read < 0 ) { */
/* 	fprintf(stderr, "failure to read.\n"); */
/* 	exit(1); */
/*     } */

/*     log_entry(writestream, SENT, buf, bytes_read); */

/*     fclose(readstream); */
/*     fclose(writestream); */
    
/*     return 0; */
/* } */
