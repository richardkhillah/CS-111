#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*include to dynamically link appropiate read_write */
#include <dlfcn.h>

#include "common.h"

int main(void) {
    fprintf(stderr, "in main\n");

    /* server handle */
    void* handle_s;
    handle_s = dlopen("./commonrw-server.so", RTLD_NOW);
    if(!handle_s)
	print_error("Error opening using dlopen");

    void (*read_write_s)(char*, char*);

    read_write_s = (void (*)(char*, char*))dlsym(handle_s, "read_write");
    if(!read_write_s) {
	print_error("Failed to link handle");
	abort();
    }
    read_write_s("testread.txt", "testserver.txt");
    
    /* client handle */
    void* handle_c;
    handle_c = dlopen("./commonrw-client.so", RTLD_NOW);
    if(!handle_c)
	print_error("Error opening using dlopen");

    void (*read_write_c)(char*, char*);

    read_write_c = (void (*)(char*, char*))dlsym(handle_c, "read_write");
    if(!read_write_c) {
	print_error("Failed to link handle");
	abort();
    }
    read_write_c("testread.txt", "testclient.txt");

    
    dlclose(handle_s);
    dlclose(handle_c);

    return 0;
}
