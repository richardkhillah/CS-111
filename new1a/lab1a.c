// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// UID: 604853262

/* msg(""); */
/* db("", ""); */
/* err(""); */

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

const int BUF_SIZE = 256;
const int TIMEOUT = 0;
const int FAIL = 1;
const int SUCCESS = 0;
int DEV_EXIT = 5;

/* getopt flags */
int shell = 0;
int debug = 0;

pid_t pid;
int pipe2shell[2];
int pipe2term[2];

/*-------- Utility functions --------*/
void msg(char* m) {
    fprintf(stderr, "%s\n", m);
}

void db(char* msg, char* func) {
    fprintf(stderr, "[debug]: %s in %s\n", msg, func);
}

void err(char* msg) {
    fprintf(stderr, "[%s %s]: ", program_name, msg);
    fprintf(stderr, "%s (errno %d).\n", strerror(errno), errno);
    exit(FAIL);
}

void dev_exit(char* m) {
    msg(m);
    exit(DEV_EXIT);
}




/*-------- Main Functions --------*/
void exec_child() {
    /* Child reads from pipe2shell[0] so close pipe2shell[1]
     * Child writes to pipe2term[1] so close pipe2term[0]
     * then redirect child process stdin, stdout, and stderr
     */

    if(debug) db("about to execl", "exec_child()");
    close(pipe2shell[1]);
    close(pipe2term[0]);

    dup2(pipe2shell[0], 0);
    dup2(pipe2term[1], 1);
    dup2(pipe2term[1], 2);

    close(pipe2shell[0]);
    close(pipe2term[1]);

    if(execl("/bin/bash", "bash", (char*)NULL) == -1)
	err("execl error");
}

void init_pipes() {
    if(pipe(pipe2shell) < 0) err("error pipe init");
    if(pipe(pipe2term) < 0) err("error pipe init");
}



/*-------- read_write functions ---------*/
/* Reading:
 * normal    : stdin -> buf
 * 
 * keyboard  : stdin -> buf
 *
 * shell     : pipe2term -> buf
 *
 * 
 * Writing read bytes:                                    Common to all
 * normal    : <cr> || <lf> -> <cr><lf> --> stdout            Y
 *           : no special char          --> stdout        X
 *
 * keybord   : <cr> || <lf> -> <cr><lf> --> stdout            Y
 *           : <cr> || <lf> -> <lf>     --> pipe2shell            -
 *           : no special char          --> stdout        x
 *           : no special char          --> pipe2shell            -
 *
 * shell     : <lf>         -> <cr><lf> --> stdout            Y*
 *           : no special char          --> stdout        x
 * 
 */

void read_write(int sh) {
    /* sh indicates whether read_write is reading shell output or not. If
     * read_write is reading shell output, sh=1, otherwise, sh=0.
     */

    const char crlf[2] = {'\r', '\n'};
    const char lf[1] = {'\n'};
    
    char buf[BUF_SIZE];
    int bytes_read = 0;
    int bytes_written = 0;
    
    while(1) {
	/* READ */

	/* Read keyboard */
	fprintf(stderr, "shell = %d", shell);
	if(!shell || (shell && sh == 0)) {
	    bytes_read = read(STDIN_FILENO, &buf, BUF_SIZE);
	    if(bytes_read < 0) {
		err("Error reading keyboard stdin");
	    }

	    /* Write buf to screen, if sh==1 forward to shell */
	    for(bytes_written = 0; bytes_written < bytes_read; bytes_written++) {
		char c = buf[bytes_written];
		
		switch (c) {
		case 0x04: // ^D
		    if (debug) db("^D", "read_write");
		    dev_exit("exiting ^D");
		    break;
		case 0x03:
		    if (debug) db("^C", "read_write");
		    if(!shell)
			exit(SUCCESS);
		    break;
		case '\r':
		    write(STDOUT_FILENO, crlf, 2);
		    if(sh)
			write(pipe2shell[1], lf, 1);
		    break;
		case '\n':
		    write(STDOUT_FILENO, crlf, 2);
		    if(sh)
			write(pipe2shell[1], lf, 1);
		    break;
		default:
		    write(STDOUT_FILENO, &c, 1);
		    if(sh)
			write(pipe2shell[1], &c, 1);
		    break;


		} // end switch		
	    } // end for
	}
	
	/* Read Shell input and echo to screen */
	else {
	    bytes_read = read(pipe2term[0], &buf, BUF_SIZE);
	    if(bytes_read < 0) {
		err("Error reading shell input");
	    }

	    for(bytes_written = 0; bytes_written < bytes_read; bytes_written++) {
		char c = buf[bytes_read];
		switch (c) {
		case '\n':
		    write(STDOUT_FILENO, crlf, 2);
		    break; 
		default:
		    write(STDOUT_FILENO, &c, 1);
		    break;
		} // end swtch
		// continues
	    }
	}
    }



	

    /* 	/\* WRITE *\/	 */
    /* 	// process each element in the buf array */
    /* 	bytes_written = 0; */
    /* 	while(bytes_written < bytes_read) { */
    /* 	    char c = buf[bytes_written]; */
	    
    /* 	    // special chars not ^D */
    /* 	    if(c == crlf[0] || c == crlf[1]) { */
    /* 	    	write(STDOUT_FILENO, crlf, 2); */
    /* 	    	if (shell && sh == 0) { // write keyboard to shell */
    /* 	    	    write(pipe2shell[1], lf, 1); */
    /* 		    //	    	    msg("would be writing(pipe2shell lf"); */
    /* 	    	} */
    /* 	    } */

    /* 	    // ^C */
    /* 	    else if (c == 0x03) { */
    /* 		if(debug) db("^C", ""); */
    /* 		kill(pid, SIGINT); */
    /* 		close(pipe2shell[1]); */
    /* 		//continue; */
    /* 		 break; */
		
    /* 		//break?? closing pipe and attempting to write to */
    /* 		// it causes poll(2) to set revents field to POLLHUP */
    /* 	    } */
	    
    /* 	    // ^D */
    /* 	    else if (c == 0x04) { */
    /* 		if(debug) db("^D", ""); */
    /* 		dev_exit("dev_exit from read_write 0x04"); */
    /* 	    } */

    /* 	    // no special chars */
    /* 	    else { */
    /* 		write(STDOUT_FILENO, &c, 1); */
    /* 		if (shell && sh == 0) { // write keyboard to shell */
    /* 		    write(pipe2shell[1], &c, 1); */
    /* 		    msg("Would be writing(pipe2shell &c"); */
    /* 		} */
    /* 	    } */
    /* 	    bytes_written++; */
    /* 	} */
    /* } */


    //XXXXXXXXXXXXXXXXXXXXXXXXXXX    
    /* while(1) { */
    /* 	int bytes_read = read(src, buf, BUF_SIZE); */
    /* 	if (bytes_read < 0) */
    /* 	    err("Error reading src"); */

    /* 	int i; */
    /* 	for(i = 0; i < bytes_read; i++){ */
    /* 	    char c = buf[i]; */
    /* 	    switch (c) { */
    /* 	    case 0x04: */
    /* 		exit(SUCCESS); */
    /* 	    case '\r': */
    /* 	    case '\n': */
    /* 		if(write(dest, "\r\n", sizeof(char)*2) < 0) */
    /* 		    err("Error writing case \\n"); */
    /* 		break; */
    /* 	    default: */
    /* 		if(write(dest, &c, sizeof(char)) < 0) */
    /* 		    err("Error writing case default"); */
    /* 		break; */
    /* 	    } */
    /* 	} */
    /* } */
}

void sig_handler(int signum) {

    if (signum == SIGINT) {
	if(debug) db("SIGINT", "sig_handler");
	dev_exit("exiting SIGINT");
	//exit(FAIL);
    }

    if (signum == SIGPIPE) {
	if(debug) db("SIGPIPE", "sig_handler");
	exit(SUCCESS);
    }
    // setup handler information.
}



/*-------- startup utilities --------*/
void get_options(int argc, char* argv[]){
    static struct option const long_opts[] = {
	{"shell", optional_argument, NULL, 's'},
	{"debug", no_argument, NULL, 'd'},
	{NULL, 0, NULL, 0}
    };

    int opt;
    int optind;
    while((opt = getopt_long(argc, (char* const*)argv, "sd", long_opts, &optind)) != -1) {
	switch (opt) {
	case 's':
	    signal(SIGINT, sig_handler);
	    signal(SIGPIPE, sig_handler);
	    shell = 1;
	    break;
	case 'd':
	    debug = 1;
	    break;
	default:
	    err("Error processing getopt");
	}
    }
}

void set_program_name(const char* argv0) {
    if(argv0 == NULL) {
	fprintf(stderr, "%s: set_program_name: %s.\n",  \
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
	exit(FAIL);
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



/*======== MAIN ========*/
int main(int argc, char* argv[]) {
    set_input_mode();
    set_program_name(argv[0]);
    get_options(argc, argv);

    if(!shell)
	read_write(0);

    /* if we made it this far, we are running the shell */
    init_pipes();
    
    pid = fork();

    if(pid < 0) {
	err("error forking");
    }

    if (pid == 0) {
	// exec_child closes all its pipes
	if(debug) db("About to exec_child", "main");
	exec_child();
    }
    
    else {
	/*close undeeded file descriptors*/
	close(pipe2shell[0]);
	close(pipe2term[1]);

	/* Create polling apparatus */
	struct pollfd pfds[2];

	pfds[0].fd = STDIN_FILENO;
	pfds[0].events = POLLIN ;
	//	pfds[0].revents = 0;

	pfds[1].fd = pipe2term[0];
	pfds[1].events = POLLIN | POLLHUP | POLLERR;
	//	pfds[0].revents = 0;

	int count = 0;
	//if(debug) db("before polling loop", "main");
	
	while(1) {
	    int ready = poll(pfds, 2, 0);

	    if (ready < 0) {
	    //if (poll(pfds, 2, TIMEOUT) < 0) {
		err("Error polling");
	    }

	    if (!ready) {
		continue;
	    }

	    // read keyboard input
	    else if (pfds[0].revents & POLLIN) {
		if(debug) db("[0].revents & POLLIN", "main");
		read_write(0);
	    }

	    // read shell input (or output, depending how you look at things
	    else if (pfds[1].revents & POLLIN) {
		if(debug) db("[1].revents & POLLIN", "main");
		read_write(1);
	    }

	    // 
	    else if (pfds[1].revents & (POLLIN | POLLHUP)) {
		dev_exit("closed pipe2term[0]");
		read_write(1);

		close(pipe2term[0]);
		
		// print_status();
		//exit()
	    }	    
	}
    }
    return 0;
}
