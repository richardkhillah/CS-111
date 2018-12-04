// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#ifndef DUMMY
#include <mraa/aio.h>
#endif

#ifdef DUMMY
// Mock implementations for local dev/test
typedef int mraa_aio_context;
int mraa_aio_init(int input) {
	input++;
	return 100;
}
int mraa_aio_read(int temp) {
	temp++;
	return 500;
}

typedef int mraa_result_t;
mraa_result_t mraa_aio_close(mraa_aio_context dev) {
    dev = 0;
    mraa_result_t ret = dev + 100;
    return ret;
}

#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "utils.h"

// include more headers here:
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <aio.h>

// lab4C unique headers
#include <netdb.h>
#include <netinet/in.h>     // so we can use sockets
#include <openssl/ssl.h>    // so we can use OpenSSL
#include <openssl/err.h>    // so we can use OpenSSL

#define EXIT_OK 0
#define EXIT_BADARG 1
#define EXIT_OTHER 2

#define B 4275
#define R0 100000
#define F 'f'
#define C 'c'
#define PERIOD_FLAG 'p'
#define LOG_FLAG 'l'
#define SCALE_FLAG 's'

#define ID_FLAG 'i'
#define HOST_FLAG 'h'

#define BUFFER_SIZE 1024
#define SSL_BUFFER_SIZE 2048

int period = 1;
char scale = F;
bool report = true;
FILE* logFile = NULL;

// New Parameters
int id = -1;
char* host = NULL;
int port = -1;

int server_socket = -1;


float getTemperature(int reading) {
    float r = 1023.0/reading-1.0;
    r = R0*r;

    float t =  1.0/(log(r/R0)/B+1/298.15)-273.15;
    if (scale == C) {
        return t;
    } else {
        return 1.8*t + 32;
    }
}

struct tm* printTime() {
    time_t rawTime;
    time(&rawTime);
    struct tm *t = localtime(&rawTime);
    return t;
}
/* We only establish_tls_cxn() when we are connecting to the
 * tls server.
 */
#ifdef TLS
char ssl_buffer[SSL_BUFFER_SIZE];
SSL *ssl;

SSL_CTX* load_tls() {
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    const SSL_METHOD* method = TLSv1_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method); // create/register method
    if(ctx == NULL) {
        // use openssl error handling to print to stdout. For more info, reference
        // https://www.openssl.org/docs/man1.1.0/crypto/ERR_print_errors_fp.html
        ERR_print_errors_fp(stderr);
        exit(EXIT_OTHER);
    }

    return ctx;
}

/* Create and initialize context and establish a connection to the remote
 * server. If we make it past establish_tls_cxn, no errors occured occured
 * and we are free to continue working.
 */
void establish_tls_cxn(SSL_CTX* ctx) {
    // must be called before any other action takes place.
    // SSL_library_init() not reentrant
    // https://www.openssl.org/docs/man1.0.2/ssl/SSL_library_init.html
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

		/* Method to use to establish a client TLS/SSL connection to a remote
		 * server using the TLSv1 protocol.
		 * This indicates that this program only understands TLSv1 and so should
		 * only receive TLSv1 messages.
		 * see SSL_CTX_new(3)
		 */
		const SSL_METHOD* method = TLSv1_client_method();
    ctx = SSL_CTX_new(method);
    if(ctx == NULL) {
        // use openssl error handling to print to stdout. For more info, reference
        // https://www.openssl.org/docs/man1.1.0/crypto/ERR_print_errors_fp.html
        ERR_print_errors_fp(stderr);
        exit(EXIT_OTHER);
    }

		// Create new ssl struct for a connection
    ssl = SSL_new(ctx);
    if(ssl == NULL) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_OTHER);
    }

		// connect the SSL object with the server_socket to enable
		// two-way IPC between local and remote machines
    if( SSL_set_fd(ssl, server_socket) == 0 ) {
            ERR_print_errors_fp(stderr);
            exit(EXIT_OTHER);
    }

		// All this work so we can establish a tls connection with the
		// remote server. A fully comprehensive error switch occurs to distinguish
		// whether an error occured client-side or server-side... which is usefull
		// when one is connecting blindly to a remote server.
		int ret_val = SSL_connect(ssl);
    if( ret_val != 1) {
        //The TLS/SSL handshake was not successful but was shut down controlled
        //and by the specifications of the TLS/SSL protocol.
        int ssl_err = SSL_get_error(ssl, ret_val);
        switch (ssl_err) {
            case SSL_ERROR_NONE:
                //fprintf(stderr, "SSL_ERROR_NONE\n");
								//exit(EXIT_OTHER);
                break;
            case SSL_ERROR_ZERO_RETURN:
                fprintf(stderr, "SSL_ERROR_ZERO_RETURN\n");
								exit(EXIT_OTHER);
                break;
            case SSL_ERROR_WANT_READ:
                fprintf(stderr, "SSL_ERROR_WANT_READ\n");
								exit(EXIT_OTHER);
            case SSL_ERROR_WANT_WRITE:
                fprintf(stderr, "SSL_ERROR_WANT_WRITE\n");
								exit(EXIT_OTHER);
                break;
            case SSL_ERROR_WANT_CONNECT:
                fprintf(stderr, "SSL_ERROR_WANT_CONNECT\n");
								exit(EXIT_OTHER);
            case SSL_ERROR_WANT_ACCEPT:
                fprintf(stderr, "SSL_ERROR_WANT_ACCEPT\n");
								exit(EXIT_OTHER);
                break;
            case SSL_ERROR_WANT_X509_LOOKUP:
                fprintf(stderr, "SSL_ERROR_WANT_X509_LOOKUP\n");
								exit(EXIT_OTHER);
                break;
            case SSL_ERROR_SYSCALL:
                fprintf(stderr, "SSL_ERROR_SYSCALL:\n");
                fprintf(stderr, "     Some non-recoverable I/O error occurred.\n");
                fprintf(stderr, "     The OpenSSL error queue may contain more information on the error.\n");
                fprintf(stderr, "     For socket I/O on Unix systems, consult errno for details\n");
                fprintf(stderr, "(errno %d): %s\n", errno, strerror(errno));
                exit(EXIT_OTHER);
                break;
        }
    } else if( ret_val < 0 ){
        fprintf(stderr, "Unable to connect to tls server\n");
        //ERR_print_errors_fp(stderr);
        exit(EXIT_OTHER);
    }
}
#endif


void shutDown() {
    struct tm* t = printTime();

#ifdef TLS
    int bytes_printed = sprintf(ssl_buffer, "%02d:%02d:%02d SHUTDOWN\n",
            t->tm_hour, t->tm_min, t->tm_sec);
    SSL_write(ssl, ssl_buffer, bytes_printed);
#else
    dprintf(server_socket, "%02d:%02d:%02d SHUTDOWN\n",
             t->tm_hour, t->tm_min, t->tm_sec);
#endif
    if (fprintf(logFile, "%02d:%02d:%02d SHUTDOWN\n", t->tm_hour,
                t->tm_min, t->tm_sec) < 0)
    {
        fprintf(stderr, "There was a problem writing to output\n");
        exit(EXIT_OTHER);
    }

    exit(EXIT_OK);
}


void usage(void) {
	fprintf(stderr, "usage: ./%s --id=9_digit_id --host=name --log=filename port [--period=seconds] [--scale=<f, c>]\n", program_name);
    fprintf(stderr, "    id: required 9 digit ID\n");
    fprintf(stderr, "    host: required name or address\n");
    fprintf(stderr, "    log: required filename to log session\n");
    fprintf(stderr, "    port: required port number\n");
    fprintf(stderr, "    period: optional, time (in seconds) between temperature sensor readings\n");
    fprintf(stderr, "            default period is 1 second\n");
    fprintf(stderr, "    scale: optional, temperature scale in degrees fahrenheit (default) or celsius.\n");
}

void getoptions(int argc, char* const* argv) {
    // The program has four required arguments, so ensure there
    // are at least 5 arguments in the program
    if(argc < 5)
        fatal_error("Missing Arguments", (void*)usage, EXIT_BADARG);

    // Find non-switch argument and assign to port number
    for(int p = 1; p < argc; p++) {
        // A switch argument begins with a '-'
        if(argv[p][0] != '-') {
            port = atoi(argv[p]);
            if(port < 0) {
                fatal_error("Port number must be greater than 0",
                            (void*)usage, EXIT_BADARG);
            }
        }
    }

	int optind = 1;
    int opt;
    struct option long_options[] = {
        {"id", required_argument, NULL, ID_FLAG},
        {"host", required_argument, NULL, HOST_FLAG},
        {"log", required_argument, NULL, LOG_FLAG},
        {"period", required_argument, NULL, PERIOD_FLAG},
        {"scale", required_argument, NULL, SCALE_FLAG},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "p:l:s:", long_options, &optind)) != -1) {
        switch (opt) {
            case ID_FLAG: {
                // Ensure id is 9 digits in length
                if (strlen(optarg) != 9){
                    fatal_error("Incorrect ID", (void*)usage, EXIT_BADARG);
                }
                id = atoi(optarg);
                if(id < 0) {
                    fatal_error("Id must be greater than 0",
                                (void*)usage, EXIT_BADARG);
                }
                break;
            }
            case HOST_FLAG: {
                host = optarg;
                if(host == NULL) {
                    fatal_error("Internal Error", NULL, -1);
                }
                break;
            }
            case PERIOD_FLAG:
                if (optarg) {
                    period = atoi(optarg);
                    if (period < 1) {
                    	fatal_error("Period must be greater-than or equal to 0",
                                    (void*)usage, 1);
                    }
                }
                break;
            case LOG_FLAG:
                if (optarg) {
                    logFile = fopen(optarg, "w");
                    if (logFile == NULL) {
                    	fatal_error("Could not open file", NULL, 1);
                    }
                }
                break;
            case SCALE_FLAG:
                if (optarg) {
                    optarg[0] = tolower(optarg[0]);
                    if (strcmp(optarg, "c") != 0 && strcmp(optarg, "f")) {
                    	fatal_error("unknown scale... scale must be \"F\" or \"C\"",
                                    (void*)usage, 1);
                    }
                    scale = tolower(optarg[0]);
                }
                break;
            case '?':
            default:
            	fatal_error("unrecognized argument", (void*)usage, 1);
        }
    }
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	getoptions(argc, (char* const*)argv);

	// ENSURE THAT WE HAVE ALL REQUIRED ARGUMENTS (ID, HOST, LOG, PORT NUMBER)
    // BEFORE CONTINUING
    if ( id == -1 || host == NULL || logFile == NULL || port == -1 ) {
        fatal_error("required_arguments not met", (void*)usage, EXIT_BADARG);
    }

	// Create a socket to communcate with remote server
    struct sockaddr_in server_address;
    struct hostent *server;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
        fatal_error("Error setting up socket", NULL, EXIT_OTHER);
    }

    /* This is the user-defined host */
    server = gethostbyname(host);
    if (server == NULL) {
        if( h_errno == HOST_NOT_FOUND) {
            fatal_error("Host not found", NULL, EXIT_OTHER);
        } else if (h_errno == TRY_AGAIN) {
            fatal_error("Try again", NULL, EXIT_OTHER);
        } else if(h_errno == NO_RECOVERY) {
            fatal_error("Non-recoverable server failure. Trying again will NOT help",
                        NULL, EXIT_OTHER);
        } else {
            fatal_error("Unknown Error... Trying again MIGHT help (it also might not)",
                        NULL, EXIT_OTHER);
        }
    }

    /* set the address */
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length);
    server_address.sin_port = htons(port);

    /* Attempt a connection */
    if (connect(server_socket, (struct sockaddr*)&server_address,
                sizeof(server_address)) < 0) {
        fatal_error("Failure connecting to server", NULL, EXIT_OTHER);
    }

    /* Create the context and ssl struct for the ssl connection.
     * If load_tls() returns, neither ssl_ctx nor ssl will not be NULL. */
#ifdef TLS
    SSL_CTX* ssl_ctx = NULL;
    establish_tls_cxn(ssl_ctx);

	// Immediately send (and log) an ID termindated with a newline:
    // ID=ID-number
    // Write only the number of bytes that is written to char string ssl_buffer
    int bytes_printed = sprintf(ssl_buffer, "ID=%d\n", id);
    SSL_write(ssl, ssl_buffer, bytes_printed);

#else // do things according to part 1
    dprintf(server_socket, "ID=%d\n", id);
#endif

    // and log
    if(fprintf(logFile, "ID=%d\n", id) < 0) {
        fatal_error("Failure to write to log", NULL, EXIT_OTHER);
    }

    mraa_aio_context temp_sensor;
    temp_sensor = mraa_aio_init(1);

    char buffer[BUFFER_SIZE];
    char temp[BUFFER_SIZE*2];
    int tempindex = 0;
    int bufferindex = 0;

    // set socket to non-block
    if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1) {
    	fatal_error("Error changing server_socket to non-blocking", NULL, EXIT_OTHER);
    }

    while (true) {
        // Send (and log) newline terminated temperature reports over the cxn
        if (report) {
            struct tm* t = printTime();
            int value = mraa_aio_read(temp_sensor);
            float tempValue = getTemperature(value);

            // Print to stdout
            printf("%02d:%02d:%02d %.1f\n", t->tm_hour, t->tm_min, t->tm_sec, tempValue);

            // Send
#ifdef TLS
            int bytes_printed = sprintf(ssl_buffer, "%02d:%02d:%02d %.1f\n",
                        t->tm_hour, t->tm_min, t->tm_sec, tempValue);
            SSL_write(ssl, ssl_buffer, bytes_printed);
#else
            if (dprintf(server_socket, "%02d:%02d:%02d %.1f\n", t->tm_hour,
                        t->tm_min, t->tm_sec, tempValue) < 0)
            {
                fatal_error("Error writing to server", NULL, EXIT_OTHER);
            }
#endif
            // and log
            if (fprintf(logFile, "%02d:%02d:%02d %.1f\n", t->tm_hour, t->tm_min,
                        t->tm_sec, tempValue) < 0) {
            	fatal_error("Error writing to logfile", NULL, EXIT_OTHER);
            }
            sleep(period);
        }

        /* process (and log) newline-terminated commands received over the
         * connection.
         * If the temperature reports are mis-formatted, the server will return
         * a LOG command with a description of the error
         */
        // Read input from either the tls server or tcp server
        int bytes_read = 0;
#ifdef TLS
        bytes_read = SSL_read(ssl, buffer+bufferindex,
                                  sizeof(char)*(BUFFER_SIZE-bufferindex));
#else
        bytes_read = read(server_socket, buffer+bufferindex,
                                  sizeof(char)*(BUFFER_SIZE-bufferindex));
#endif
        if (bytes_read < 0) {
                int err = errno;
                if (err != EAGAIN) {
                    fatal_error("There was an issue reading from the server",
								NULL, EXIT_OTHER);
                }
            }

        // and Process (the same as before i.e., in part 4b)
        if (bytes_read > 0) {
            bufferindex += bytes_read;
            int i;
            for (i = 0; i < bufferindex; i++) {
                if (buffer[i] == '\n') {
                    temp[tempindex] = '\0';
                    char* pposition = strstr(temp, "PERIOD=");
                    if (strcmp(temp, "SCALE=F") == 0) {
                        scale = F;
                    } else if (strcmp(temp, "SCALE=C") == 0) {
                        scale = C;
                    } else if (strcmp(temp, "OFF") == 0) {
                        if (fprintf(logFile, "%s\n", temp) < 0) {
                        	fatal_error("Error writing to logfile", NULL, EXIT_OTHER);
                        }
                        shutDown();
                    } else if (strcmp(temp, "STOP") == 0) {
                        report = false;
                    } else if (strcmp(temp, "START") == 0) {
                        report = true;
                    } else if (pposition != NULL && pposition == temp && strlen(temp) > 7) {
                        int num = atoi(pposition+7);
                        if (num >= 1) {
                            period = num;
                        }
                    }

                    // and log
                    if (fprintf(logFile, "%s\n", temp) < 0) {
                    	fatal_error("Error writing to logfile", NULL, EXIT_OTHER);
                    }

                    tempindex = 0;
                } else {
                    temp[tempindex++] = buffer[i];
                }
        }

            bufferindex = 0;
        }
    }

    mraa_aio_close(temp_sensor);
    close(server_socket);
#ifdef TLS
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
#endif

}
