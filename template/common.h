// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

/**
 * set_program_name (and program_name) .. set program name using 
 * argv[0] of command line argument.
 * 
 * The program strips the './' from the command used to invoke the 
 * function and stores the result in program_name.
 *
 * @param const char* argv0 ... main routines argv[0]. 
 */
const char* program_name;

void set_program_name(const char* argv0);


// common exit status codes
#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

/**
 * 
 */
void fatal_error(const* msg);

/**
 * 
 */
void handle_error(char* msg);

/**
 * 
 */
void debug(char* msg);

/**
 *
 */
//TODO: Add @param's
//TODO: Add verdaic from https://stackoverflow.com/questions/27795767/optional-arguments-in-c-function