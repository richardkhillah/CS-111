# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262
LAB_NUMBER=lab4
LAB_PART=b
EXECUTABLE_NAME=$(LAB_NUMBER)$(LAB_PART)
EXECUTABLE_SOURCE=$(EXECUTABLE_NAME).c
#CHECK_SCRIPT=P4B_check.sh
CHECK_SCRIPT=my_check.sh


CC=gcc

CFLAGS= -Wall -Wextra -std=gnu99 -lm 
local_flags= $(CFLAGS) -DDUMMY 
beagle_flags= $(CFLAGS) -lmraa 

DEFAULT=Makefile README utils.c utils.h

EXTRAS=

BEAGLEBONE_VERSION=$(uname -a | grep 'lasr.cs.ucla.edu')

default: build

build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
#ifeq ($(BEAGLEBONE_VERSION), 1)
	$(CC) $(beagle_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c
#else
#	$(CC) $(local_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c
#endif


dist: build
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS) $(CHECK_SCRIPT)

clean:
	rm -rf *tar.gz

check:
	./$(CHECK_SCRIPT)
