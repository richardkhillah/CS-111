# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262
LAB_NUMBER=lab4
LAB_PART=b
EXECUTABLE_NAME=$(LAB_NUMBER)$(LAB_PART)
EXECUTABLE_SOURCE=$(EXECUTABLE_NAME).c
TEST_SCRIPT=
CHECK_SCRIPT=P4B_check.sh


CC=gcc

CFLAGS= -Wall -Wextra -std=gnu99

DEFAULT=Makefile README utils.c utils.h

EXTRAS=


default: build

build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
	$(CC) $(CFLAGS) -o $@ $(EXECUTABLE_SOURCE) utils.c


dist: build
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS) $(TEST_SCRIPT)

clean:
	rm -rf *tar.gz

test:
	./$(TEST_SCRIPT)

check:
	./$(CHECK_SCRIPT)
