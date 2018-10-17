#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#include "common.h"

//struct termios saved_attributes;

int main(int argc, char* argv[]) {
    set_keyboard(CLIENT);
    
    struct options* opts;
    opts = get_options(argc, argv, CLIENT);

    return 0;
}
