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
#CFLAGS= -Wall -Wextra -std=gnu99 -lm
CFLAGS_LMRAA= -lmraa
DUMMY= -D DUMMY

DEFAULT=Makefile README utils.c utils.h

EXTRAS=

#########
local_flags= -Wall -Wextra -std=gnu99 -lm -DDUMMY
beagle_flags= -Wall -Wextra -std=gnu99 -lm -lmraa 

bb=$(uname -r | grep beaglebone)

ifeq ($bb, 0)
	CFLAGS=$(local_flags)
else
	CFLAGS=$(beagle_flags)
endif

#########

default: build

build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
	$(CC) $(cflags) -o $@ $(EXECUTABLE_SOURCE) utils.c

dist: build
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS)

clean:
	rm -rf $(EXECUTABLE_NAME) *tar.gz

check:
	./$(CHECK_SCRIPT) $(UID)

tests: dist
	./$(MY_CHECK_SCRIPT)
	