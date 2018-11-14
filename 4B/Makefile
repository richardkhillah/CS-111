# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262
LAB_NUMBER=lab4
LAB_PART=b
EXECUTABLE_NAME=$(LAB_NUMBER)$(LAB_PART)
EXECUTABLE_SOURCE=$(EXECUTABLE_NAME).c
CHECK_SCRIPT=P4B_check.sh
MY_CHECK_SCRIPT=my_check.sh
TEMP=./"tmp"


CC=gcc
CFLAGS= -Wall -Wextra -std=gnu99 -lm
local_flags= $(CFLAGS) -D DUMMY -D DEV
dev_flags=$(local_flags) -D DEV
beagle_flags= $(CFLAGS) -lmraa 

DEFAULT=Makefile README utils.c utils.h

EXTRAS=

BEAGLEBONE_VERSION=$(uname -r | grep 'beaglebone')
DEV_VERSION=$(uname | grep 'Linux')

default: build

build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
# ifeq ($(DEV_VERSION), "Linux")
# 	$(CC) $(dev_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c
# else 
ifeq ($(BEAGLEBONE_VERSION),)
	$(CC) $(local_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c
else
	$(CC) $(beagle_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c
endif


dist: build
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS)

clean:
	rm -rf $(EXECUTABLE_NAME) *tar.gz

check:
	./$(CHECK_SCRIPT) $(UID)

tests: dist
	./$(MY_CHECK_SCRIPT)