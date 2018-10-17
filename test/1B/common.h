#define EXIT_ERR 1
#define EXIT_OK 0
#define SERVER 1
#define CLIENT 0

/* structure containing 5 possible commandline options:
 * --port, --log, --encrypt, --host, and --debug. */
struct optpair { int flag; char* arg; };

struct options {
    struct optpair port;
    struct optpair log;
    struct optpair encrypt;
    struct optpair host;
    int debug;
};

//extern struct options {};

/* get_options parses command line options, filling an options struct
 * with the results. it takes argc passed in from main, argv, also
 * passed in though main, and mode. mode is either 1 for SERVER mode,
 * or 0 for CLIENT mode. If get_options is running in SERVER mode, it
 * will not accept arguments --log or --host. An error will print to
 * stdout and the program will exit(2) with return code 1.
 */
extern struct options* get_options(int argc, char* argv[], int mode);

/* set keyboard enables the user to set the keyboard into non-canonical
 * no-echo mode.
 */
//extern void set_keyboard(struct termios* saved_attributes, int mode);
extern void set_keyboard(int mode);
