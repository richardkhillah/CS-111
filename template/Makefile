# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262
LAB_NUMBER=lab
LAB_PART=
EXECUTABLE_NAME=$(LAB_NUMBER)$(LAB_PART)
EXECUTABLE_SOURCE=
CHECK_SCRIPT=

CC=gcc
CFLAGS= -Wall -Wextra -std=gnu99

DEFAULT=Makefile README utils.c utils.h

EXTRAS=

default: build

build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
	$(CC) $(CFLAGS) -o $@ $(EXECUTABLE_SOURCE) utils.c

dist:
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS)

clean:
	rm -rf $(EXECUTABLE_NAME) *tar.gz

check:
	./$(CHECK_SCRIPT) $(UID)

tests:
