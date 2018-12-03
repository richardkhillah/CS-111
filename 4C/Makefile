# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

FAKEID=369713302

UID=604853262
LAB_NUMBER=lab4
LAB_PART=c
EXECUTABLE_NAME=$(LAB_NUMBER)$(LAB_PART)
EXECUTABLE_SOURCE=$(EXECUTABLE_NAME).c
#CHECK_SCRIPT=P4C_check.sh
CHECK_SCRIPT=my_check.sh

CC=gcc
CFLAGS= -Wall -Wextra -std=gnu99 -lm -lcrypto -lssl
OPENSSL = -I /usr/local/opt/openssl/include
local_flags= $(CFLAGS) $(OPENSSL) -DDUMMY 
beagle_flags= $(CFLAGS) -lmraa 

DEFAULT=Makefile README utils.c utils.h

default: build

build: $(EXECUTABLE_NAME)_tcp

$(EXECUTABLE_NAME)_tcp:
	# $(CC) $(beagle_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c
	$(CC) $(local_flags) -o $@ $(EXECUTABLE_SOURCE) utils.c

$(EXECUTABLE_NAME)_tls:
	# $(CC) $(beagle_flags) -DTLS -o $@ $(EXECUTABLE_SOURCE) utils.c
	$(CC) $(local_flags) -DTLS -o $@ $(EXECUTABLE_SOURCE) utils.c

dist: build
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS) $(CHECK_SCRIPT)

clean:
	rm -rf *tar.gz $(EXECUTABLE_NAME)_tcp $(EXECUTABLE_NAME)_tls

check:
	./$(CHECK_SCRIPT) $(FAKEID)
