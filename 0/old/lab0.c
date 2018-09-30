#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>

#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>

const char *program_name;

static struct option const long_opts[] =
    {
	{"input", required_argument, NULL, 'i'},
	{"output", required_argument, NULL, 'o'},
	{"segfault", no_argument, NULL, 's'},
	{"catch", no_argument, NULL, 'c'},
	//	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0}
    };

struct option_vals {
    char* input;
    char* output;
    int seg;
    int catch;
};

void
set_program_name(const char *argv0){
    if(argv0 == NULL) {
	fprintf(stderr, "NULL program name passed to set_program_name.");
	abort();
    }

    const char *slash;
    const char *base;
    slash = strrchr(argv0, '/');
    base = slash != NULL ? slash+1 : argv0;
    argv0 = base;
    program_name = argv0;
}

struct option_vals*
init_option_vals(void) {
    struct option_vals* config = malloc(sizeof *config);
    
    config->input = NULL;
    config->output = NULL;
    config->seg = -1;
    config->catch = -1;
    return config;
}

void
usage() {

}

//typedef void (sighandler_t)(int);
//sighandler_t
void
segfault_handler(int signum){
    // log an error message on stderr
    // exit wit return code 4
    exit(4); // ...caught and received SIGSEGV

}

int
main(int argc, char* argv[]) {
    set_program_name(argv[0]);

    struct option_vals* config = init_option_vals();
    
    int c;
    char* optarg = NULL;
    int optind = 0;
    int optopt;
    int opterr;

    while((c = getopt_long(argc, argv, "+i:o:sch", long_opts, &optind)) != -1) {
	switch(c) {
	case 'i':
	    printf("option i\n");
	    config->input = optarg;
	    break;
	case 'o':
	    printf("option o\n");
	    config->output = optarg;
	    break;
	case 's':
	    printf("option s\n");
	    config->seg = 1;
	    break;
	case 'c':
	    printf("option c\n");
	    config->catch = 1;
	    break;
	case 'h':
	    printf("option h\n");
	    // display the help message.
	    break;
	case '?':
	    // display error.
	    exit(1); // unrecognized argument
	default:
	    // dispaly usage
	    usage();
	    break;
	}
    }

    // 1. do any file redirection
    int ifd;
    if(config->input) {
	//       (ifd = open(config->input, O_RDONLY)) ) {
	printf("config->input\n");
	ifd = open(config->input, O_RDONLY);
	if(ifd){
	printf("assigning ift to fd_0 now.\n");
	close(0); // want to use close(stdin);
	dup(ifd);
	close(ifd);
	}
    } else {
	// there was an error, figure it out and print it out then exit
	exit(2); // ...unable to open input file
    }

    int ofd;
    if((ofd = open(config->output, O_CREAT,
		   S_IRUSR | S_IWUSR | S_ISUID)) >= -1){
	close(1);// want to use close(stdout);
	dup(ofd);
	close(ofd);
    } else {
	// there was an error, figure it out and print it out then exit.
	exit(3); // ...unable to open output file
    }
    
    // 2. register the signal handler
    if(config->catch) {
	if(signal(SIGSEGV, segfault_handler) == SIG_ERR){
	    // print error message
	}
    }
    // 3. cause the segfault
    if(config->seg){
	char *p = NULL;
	*p = 'n';
    }
    // 4. if no segfault was caused, copy stdin to stdout
    char buf;
    while(read(0, &buf, sizeof(char)) > EOF) {
	write(1, &buf, sizeof(char));
    }

    // if there's an error:
    //   fprintf(stderr, "%s: %s: %s", program_name, optarg, strerror(errno));


    return 0;
}
